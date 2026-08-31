import sys
import time
import os
import re
import subprocess as sp
from utils import *

BENCHMARK_TEST_DIR = BENCH_TOP_DIR + '/test'
TIMEOUT = 600

_CONFIG_VAR_RE = re.compile(r'\$\{?([A-Za-z_][A-Za-z0-9_]*)\}?')


def read_config(hw_target:str) -> dict:
    """Parse hw/<hw_target>/config.env (shell-style KEY=VALUE).

    Mirrors hw/read_config.cmake and the `set -a; . config.env` that
    build_picolibc.sh does, so all three consumers see identical values.
    Values may reference keys defined earlier in the file, or environment
    variables, as $VAR or ${VAR}. $BENCHMARKS_ROOT is seeded here.
    """
    config_file = os.path.join(BENCH_TOP_DIR, 'hw', hw_target, 'config.env')
    if not os.path.exists(config_file):
        raise FileNotFoundError(f'Cannot find config.env at {config_file}')

    config = {'BENCHMARKS_ROOT': os.path.abspath(BENCH_TOP_DIR)}

    with open(config_file, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#') or '=' not in line:
                continue

            key, _, val = line.partition('=')
            key, val = key.strip(), val.strip()

            if len(val) >= 2 and val[0] == val[-1] and val[0] in ('"', "'"):
                val = val[1:-1]

            # Prefer keys already read from this file, fall back to the
            # environment (this is how $HOME would resolve).
            val = _CONFIG_VAR_RE.sub(
                lambda m: config.get(m.group(1), os.environ.get(m.group(1), '')),
                val)

            # Drop any trailing slash so "${VAR}/file" never doubles up.
            config[key] = val.rstrip('/')

    return config

# Single source of truth, shared with the CMake build and build_picolibc.sh.
# The firmware gets the same BAUD_RATE as -DBAUD_RATE, so the two sides of the
# link cannot drift apart.
TIVAC_CONFIG = read_config('tm4c123')
UART_DEVICE = TIVAC_CONFIG['UART_DEVICE']
BAUD_RATE = int(TIVAC_CONFIG['BAUD_RATE'])
BENCHMARK_COMPLETION_MSG = 'Benchmark execution complete.'
UART_SYNC_MARKER = '<<<BENCHMARK_START>>>'
CHECKSUM_FILE_SIZE = 10

class VerificationMethod(Enum):
    PRINTF = 'printf'
    CHECKSUM = 'checksum'


def strip_uart_preamble(output_file:str) -> None:
    """Drop the sacrificial preamble the firmware emits before its real output.

    A fixed 8-byte chunk goes missing from the head of every capture, so the
    firmware leads with filler plus UART_SYNC_MARKER and we resynchronize here.
    If the marker is absent the file is left untouched, so a firmware without
    the preamble still verifies normally instead of being silently mangled.
    """
    with open(output_file, 'r', errors='replace') as f:
        content = f.read()

    idx = content.find(UART_SYNC_MARKER)
    if idx == -1:
        return

    # Also consume the rest of the marker's line, including its newline.
    eol = content.find('\n', idx)
    content = content[eol + 1:] if eol != -1 else ''

    with open(output_file, 'w') as f:
        f.write(content)

    return


def wait_for_screen_session(session_name:str, timeout:float=10.0) -> bool:
    """Block until `session_name` shows up in `screen -ls`, or timeout."""
    deadline = time.time() + timeout
    while time.time() < deadline:
        proc = sp.run(['screen', '-ls'], capture_output=True, text=True)
        if session_name in proc.stdout:
            # The session exists, but screen still has to open the tty and set
            # the line discipline. Give it a moment to finish.
            time.sleep(0.5)
            return True
        time.sleep(0.05)

    return False


def test_benchmark_tivac(bin_file:str, output_dir:str, verif_method:VerificationMethod) -> None:
    benchmark = os.path.basename(bin_file).replace('.bin', '')
    print(f'{benchmark:>13}: ', end='')
    sys.stdout.flush()

    output_file = f'{output_dir}/{benchmark}'
    os.remove(output_file) if os.path.exists(output_file) else None
    screen_session_name = f'{benchmark}_screen'
    cmd = f'screen -DmS {screen_session_name} -L -Logfile {output_file} {UART_DEVICE} {BAUD_RATE}'
    proc = sp.Popen(cmd, shell=True)

    # screen is started asynchronously, but lm4flash releases the target from
    # reset as soon as it finishes, and the benchmark starts printing straight
    # away. Flashing before screen has opened the port -- and applied its
    # termios settings, which flush the tty input queue -- silently eats the
    # start of the output. Wait for the session to actually come up first.
    if not wait_for_screen_session(screen_session_name):
        print(color('screen session did not start. ', 'red'), end='')
        sys.stdout.flush()

    cmd = f'lm4flash {bin_file}'
    proc = sp.run(cmd, shell=True, capture_output=True, text=True)
    if proc.returncode != 0:
        print(color(f'lm4flash failed.', 'red'))
    print(color('Flashing complete. ', 'green', 'd'), end='')
    sys.stdout.flush()

    exec_time = 0
    benchmark_completed = False
    while exec_time < TIMEOUT:
        time.sleep(1)
        exec_time += 1
        if os.path.exists(output_file):
            if verif_method == VerificationMethod.PRINTF:
                with open(output_file, 'r') as f:
                    content = f.read()
                    if BENCHMARK_COMPLETION_MSG in content:
                        benchmark_completed = True
                        break
            else:  # VerificationMethod.CHECKSUM
                if os.path.getsize(output_file) >= CHECKSUM_FILE_SIZE:
                    benchmark_completed = True
                    break

    sp.run(["screen", "-S", screen_session_name, "-X", "quit"])

    if not benchmark_completed:
        print(color('Timed out', 'red', 'b'))
        return
    else:
        strip_uart_preamble(output_file)
        verify_output(benchmark, output_dir, verif_method)

    return


def test_all_tivac(bin_dir:str, output_dir:str, verif_method:VerificationMethod):
    for fname in os.listdir(bin_dir):
        if fname.endswith('.bin'):
            test_benchmark_tivac(os.path.join(bin_dir, fname), output_dir, verif_method)

    return


def verify_output(benchmark:str, output_dir:str, verif_method:VerificationMethod) -> None:
    test_output_file = f'{output_dir}/{benchmark}'

    if verif_method == VerificationMethod.PRINTF:
        gold_output_file = f'{BENCHMARK_TEST_DIR}/reference_output/{benchmark}'
        cmd = f'python3 {BENCHMARK_TEST_DIR}/check_output.py {gold_output_file} {test_output_file} -d 0.01'
        proc = sp.run(cmd, shell=True, capture_output=True, text=True)

        if proc.stdout.strip() == 'Files match.':
            print(color('Passed', 'green', 'b'))
        else:
            print(color('Failed:', 'red', 'b'))
            print(proc.stdout)

    elif verif_method == VerificationMethod.CHECKSUM:
        gold_output_file = f'{BENCHMARK_TEST_DIR}/checksums/{benchmark}'
        with open(gold_output_file, 'r') as f:
            gold_checksum = f.read().strip()
        with open(test_output_file, 'r') as f:
            test_checksum = f.readlines()[0].strip()

        if gold_checksum == test_checksum:
            print(color('Passed', 'green', 'b'))
        else:
            print(color('Failed:', 'red', 'b'))
            print(f'Expected checksum: {gold_checksum}    Got checksum: {test_checksum}')



def main():
    parser = argparse.ArgumentParser(parents=[get_parent_parser()], description='Test benchmarks on TivaC')
    parser.add_argument('-m', '--method', type=str, choices=['printf', 'checksum'], default='printf', help='Verification method to use (default: printf)')
    args = parser.parse_args()

    if not args.input or not args.output:
        print(color("Input and output directories must be specified. Exiting...", 'red'))
        sys.exit(1)
    check_dir_exists(args.output, create=True)

    if os.path.isfile(args.input):
        test_benchmark_tivac(args.input, args.output, VerificationMethod(args.method))
    elif check_dir_exists(args.input, create=False):
        test_all_tivac(args.input, args.output, VerificationMethod(args.method))
    else:
        print('Invalid input. Please provide a valid mem file or directory containing mem files.')

    return


if __name__ == '__main__':
    main()
