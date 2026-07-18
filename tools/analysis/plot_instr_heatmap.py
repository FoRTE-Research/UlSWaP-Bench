from matplotlib.ticker import StrMethodFormatter

from utils import *
from plot_utils import *


def get_instruction_count_maps(dir_name:str) -> dict[str, dict[str, int]]:
    benchmark_maps = {}

    for filename in os.listdir(dir_name):
        filepath = os.path.join(dir_name, filename)
        instr_count_map = {}
        benchname = filename.split('.')[0]
        with open(filepath, 'r') as fp:
            csv = fp.read()

        for line in csv.splitlines():
            instr, count = line.split(',')
            instr_count_map[instr] = int(count)

        benchmark_maps[benchname] = instr_count_map

    benchmark_maps = dict(sorted(benchmark_maps.items(), key=lambda item: item[0]))

    return benchmark_maps


def get_hw_count_maps(instr_count_maps:dict[str, dict[str, int]]) -> dict[str, dict[str, int]]:
    hw_count_map = {}
    for benchmark, instr_count_map in instr_count_maps.items():
        hw_count_map[benchmark] = instr_count_map.copy()
        for instr, count in instr_count_map.items():
            if instr.endswith('I') and instr != 'LUI':
                hw_instr = instr[:-1]
                hw_count_map[benchmark][hw_instr] = hw_count_map[benchmark].get(hw_instr, 0) + count
                hw_count_map[benchmark].pop(instr)

    return hw_count_map


def get_normalized_instruction_count_maps(instr_count_maps:dict[str, dict[str, int]]) -> dict[str, dict[str, float]]:
    normalized_maps = {}
    for benchmark, instr_count_map in instr_count_maps.items():
        normalized_map = {k: v / sum(instr_count_map.values()) for k, v in instr_count_map.items()}
        normalized_maps[benchmark] = normalized_map

    return normalized_maps



def heatmap(data:np.ndarray,
            row_labels:list[str],
            col_labels:list[str],
            ax:plt.Axes,
            cbar_kw:dict|None=None,
            cbarlabel:str="",
            **kwargs):
    """
    Create a heatmap from a numpy array and two lists of labels.

    Parameters
    ----------
    data
        A 2D numpy array of shape (M, N).
    row_labels
        A list or array of length M with the labels for the rows.
    col_labels
        A list or array of length N with the labels for the columns.
    ax
        A `matplotlib.axes.Axes` instance to which the heatmap is plotted.
    cbar_kw
        A dictionary with arguments to `matplotlib.Figure.colorbar`.  Optional.
    cbarlabel
        The label for the colorbar.  Optional.
    **kwargs
        All other arguments are forwarded to `imshow`.
    """

    FONT_SIZE = 7.5

    if cbar_kw is None:
        cbar_kw = {}

    # Plot the heatmap
    im = ax.imshow(data, **kwargs)

    # Create colorbar
    cbar = ax.figure.colorbar(im, ax=ax, fraction=0.025, pad=0.01, aspect=70, **cbar_kw)
    cbar.ax.set_ylabel(cbarlabel, rotation=-90, va="bottom", fontsize=FONT_SIZE)
    cbar.ax.tick_params(labelsize=FONT_SIZE)


    # Show all ticks and label them with the respective list entries.
    ax.set_xticks(np.arange(data.shape[1]), labels=col_labels, fontsize=FONT_SIZE)
    ax.set_yticks(np.arange(data.shape[0]), labels=row_labels, fontsize=FONT_SIZE)

    # Let the horizontal axes labeling appear on top.
    ax.tick_params(top=False, bottom=True,
                   labeltop=False, labelbottom=True)

    # Rotate the tick labels and set their alignment.
    plt.setp(ax.get_xticklabels(), rotation=90)
            #   ha="right", rotation_mode="anchor")

    # Turn spines off and create white grid.
    # ax.spines[:].set_visible(False)

    ax.set_xticks(np.arange(data.shape[1]+1)-.5, minor=True)
    ax.set_yticks(np.arange(data.shape[0]+1)-.5, minor=True)
    ax.grid(which="minor", color="w", linestyle='-', linewidth=2)
    ax.tick_params(which="minor", bottom=False, left=False)

    return im, cbar


def annotate_heatmap(im, data:np.ndarray, valfmt="{x:.2f}",
                     textcolors=("black", "white"),
                     threshold=None, **textkw):
    """
    A function to annotate a heatmap.

    Parameters
    ----------
    im
        The AxesImage to be labeled.
    data
        Data used to annotate.
    valfmt
        The format of the annotations inside the heatmap.  This should either
        use the string format method, e.g. "$ {x:.2f}", or be a
        `matplotlib.ticker.Formatter`.  Optional.
    textcolors
        A pair of colors.  The first is used for values below a threshold,
        the second for those above.  Optional.
    threshold
        Value in data units according to which the colors from textcolors are
        applied.  If None (the default) uses the middle of the colormap as
        separation.  Optional.
    **kwargs
        All other arguments are forwarded to each call to `text` used to create
        the text labels.
    """

    if not isinstance(data, (list, np.ndarray)):
        data = im.get_array()

    # Normalize the threshold to the images color range.
    if threshold is not None:
        threshold = im.norm(threshold)
    else:
        threshold = im.norm(data.max())/2.

    # Set default alignment to center, but allow it to be
    # overwritten by textkw.
    kw = dict(horizontalalignment="center",
              verticalalignment="center")
    kw.update(textkw)

    # Get the formatter in case a string is supplied
    if isinstance(valfmt, str):
        valfmt = StrMethodFormatter(valfmt)

    # Loop over the data and create a `Text` for each "pixel".
    # Change the text's color depending on the data.
    texts = []
    for i in range(data.shape[0]):
        for j in range(data.shape[1]):
            if data[i, j] != 0:
                continue
            kw.update(color=textcolors[int(im.norm(data[i, j]) > threshold)])
            text = im.axes.text(j, i, valfmt(data[i, j], None), **kw)  # type: ignore[arg-type]
            texts.append(text)

    return texts


def show_heatmap(instr_count_maps:dict[str, dict[str, float]], image_name:str|None) -> None:
    FONT_SIZE = 7.5
    FIGURE_HEIGHT = 3.5
    TICK_PADDING = 2
    ZERO_FONT_SIZE = 5

    fig, ax = plt.subplots(constrained_layout=True)
    fig.set_figwidth(PAGEWIDTH)
    fig.set_figheight(FIGURE_HEIGHT)

    benchmark_names = list(instr_count_maps.keys())
    instr_names = set()
    for instr_count_map in instr_count_maps.values():
        instr_names.update(instr_count_map.keys())
    instr_names = list(instr_names)
    instr_names.sort()

    heatmap_data = np.zeros((len(benchmark_names), len(instr_names)))
    for i, benchmark_name in enumerate(benchmark_names):
        instr_count_map = instr_count_maps[benchmark_name]
        for j, instr_name in enumerate(instr_names):
            heatmap_data[i, j] = instr_count_map.get(instr_name, 0)

    im, cbar = heatmap(heatmap_data, benchmark_names, instr_names, ax, cbarlabel='Relative Instruction Freqruency', cmap='gray_r')
    annotate_heatmap(im, data=heatmap_data, valfmt='{x:.0f}', fontsize=ZERO_FONT_SIZE)

    if image_name is not None:
        plt.savefig(image_name)
        crop_pdf(image_name)
    else:
        plt.show()

    return


help_msg = '''
Generate heatmap of dynamic instruction frequencies for each benchmark.
The input directory should contain CSV files mapping each instruction to its count.
The '--hw' flag can be used to merge the counts for immediate and register instructions for RISC-V.
If no output file is specified, the plot will be displayed.
'''

def main():
    parent_parser = get_parent_parser(True, False)
    parser = argparse.ArgumentParser(parents=[parent_parser], description=help_msg)
    parser.add_argument('--hw', action='store_true', help='Create hardware view heatmap (merge immediate and register instructions). RISC-V only.')

    args = parser.parse_args()

    check_dir_exists(args.input, create=False)

    instr_count_maps = get_instruction_count_maps(args.input)
    if args.hw:
        instr_count_maps = get_hw_count_maps(instr_count_maps)

    normalized_maps = get_normalized_instruction_count_maps(instr_count_maps)
    show_heatmap(normalized_maps, args.output)

    return


if __name__ == '__main__':
    main()
