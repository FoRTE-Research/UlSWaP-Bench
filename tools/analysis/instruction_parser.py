import sys
import pickle

class RISCVInstruction:
    def __init__ (self, hex_val, operation, rd, rs1, rs2, imm):
        self.hex_val = hex_val
        self.operation = operation
        self.rd = rd
        self.rs1 = rs1
        self.rs2 = rs2
        self.imm = imm
        self.address = None


    def set_address(self, address):
        self.address = address


    def __unique_id(self):
        return self.operation # + ' ' + str(self.rd)

    def __str__(self):
        return self.__unique_id()


    # def __str__(self):
    #     if self.rs2 is None:
    #         return f'{self.operation} {self.rd} {self.rs1} {self.imm}'
    #     else:
    #         return f'{self.operation} {self.rd} {self.rs1} {self.rs2}'


    def __eq__(self, other):
        return self.operation == other.operation


    def __hash__(self):
        return hash(self.__unique_id())


# Define RISC-V instruction dictionary
RISCV_INSTRUCTION_DICT = {
    # U-type instructions
    ('0110111', None, None): 'LUI',
    ('0010111', None, None): 'AUIPC',

    # R-type instructions
    ('0110011', '000', '0000000'): 'ADD',
    ('0110011', '000', '0100000'): 'SUB',
    ('0110011', '001', '0000000'): 'SLL',
    ('0110011', '010', '0000000'): 'SLT',
    ('0110011', '011', '0000000'): 'SLTU',
    ('0110011', '100', '0000000'): 'XOR',
    ('0110011', '101', '0000000'): 'SRL',
    ('0110011', '101', '0100000'): 'SRA',
    ('0110011', '110', '0000000'): 'OR',
    ('0110011', '111', '0000000'): 'AND',

    # I-type instructions
    ('0010011', '000', None): 'ADDI',
    ('0010011', '010', None): 'SLTI',
    ('0010011', '011', None): 'SLTIU',
    ('0010011', '100', None): 'XORI',
    ('0010011', '110', None): 'ORI',
    ('0010011', '111', None): 'ANDI',
    ('0010011', '001', None): 'SLLI',
    ('0010011', '101', '0000000'): 'SRLI',
    ('0010011', '101', '0100000'): 'SRAI',

    # Load instructions
    ('0000011', '000', None): 'LB',
    ('0000011', '001', None): 'LH',
    ('0000011', '010', None): 'LW',
    ('0000011', '100', None): 'LBU',
    ('0000011', '101', None): 'LHU',

    # Store instructions
    ('0100011', '000', None): 'SB',
    ('0100011', '001', None): 'SH',
    ('0100011', '010', None): 'SW',

    # Branch instructions
    ('1100011', '000', None): 'BEQ',
    ('1100011', '001', None): 'BNE',
    ('1100011', '100', None): 'BLT',
    ('1100011', '101', None): 'BGE',
    ('1100011', '110', None): 'BLTU',
    ('1100011', '111', None): 'BGEU',

    # Jump instructions
    ('1101111', None, None): 'JAL',
    ('1100111', None, None): 'JALR',

    # M-Extension instructions
    ('0110011', '000', '0000001'): 'MUL',
    ('0110011', '001', '0000001'): 'MULH',
    ('0110011', '010', '0000001'): 'MULHSU',
    ('0110011', '011', '0000001'): 'MULHU',
    ('0110011', '100', '0000001'): 'DIV',
    ('0110011', '101', '0000001'): 'DIVU',
    ('0110011', '110', '0000001'): 'REM',
    ('0110011', '111', '0000001'): 'REMU',
}

RISCV_INSTRUCTIONS = RISCV_INSTRUCTION_DICT.values()


def disassemble_riscv_op(hex_value:str) -> str|None:
    # Convert hexadecimal value to 32-bit binary string
    binary_value = bin(int(hex_value, 16))[2:].zfill(32)

    # Define RISC-V instruction fields
    opcode = binary_value[-7:]
    funct3 = binary_value[-15:-12]
    funct7 = binary_value[:7]

    # Match opcode, funct3, and funct7 to an instruction
    for key, value in RISCV_INSTRUCTION_DICT.items():
        if key[0] == opcode:
            if key[1] == funct3 or key[1] is None:
                if key[2] == funct7 or key[2] is None:
                    operation = value
                    break
    else:
        print(f'Unknown instruction: {binary_value}')
        operation = 'Unknown Instruction'

    return operation


def disassemble_riscv_full(hex_value:str) -> RISCVInstruction|None:
    # Convert hexadecimal value to 32-bit binary string
    binary_value = bin(int(hex_value, 16))[2:].zfill(32)

    # Define RISC-V instruction fields
    opcode = binary_value[-7:]
    funct3 = binary_value[-15:-12]
    funct7 = binary_value[:7]
    rs1 = binary_value[-20:-15]
    rs2 = binary_value[-25:-20]
    rd = binary_value[-12:-7]

    # Match opcode, funct3, and funct7 to an instruction
    for key, value in RISCV_INSTRUCTION_DICT.items():
        if key[0] == opcode:
            if key[1] == funct3 or key[1] is None:
                if key[2] == funct7 or key[2] is None:
                    operation = value
                    break
    else:
        print(f'Unknown instruction: {hex_value}')
        operation = 'Unknown Instruction'

    # Generate the disassembled instruction
    # ALU register-immediate instructions
    if operation in ['ADD', 'SUB', 'SLL', 'SLT', 'SLTU', 'XOR', 'SRL', 'SRA', 'OR', 'AND']:
        instruction = RISCVInstruction(hex_value, operation, f'x{int(rd, 2)}', f'x{int(rs1, 2)}', f'x{int(rs2, 2)}', None)
    # ALU register-immediate instructions
    elif operation in ['ADDI', 'SLTI', 'SLTIU', 'XORI', 'ORI', 'ANDI', 'SLLI', 'SRLI', 'SRAI']:
        instruction = RISCVInstruction(hex_value, operation, f'x{int(rd, 2)}', f'x{int(rs1, 2)}', None, hex(int(binary_value[0:12], 2)))
    # Load instructions
    elif operation in ['LB', 'LH', 'LW', 'LBU', 'LHU']:
        instruction = RISCVInstruction(hex_value, operation, f'x{int(rd, 2)}', f'x{int(rs1, 2)}', None, hex(int(binary_value[0:12], 2)))
    # Store instructions
    elif operation in ['SB', 'SH', 'SW']:
        imm = binary_value[0:7] + binary_value[20:25]
        instruction = RISCVInstruction(hex_value, operation, None, f'x{int(rs1, 2)}', f'x{int(rs2, 2)}', hex(int(imm, 2)))
    # Branch instructions
    elif operation in ['BEQ', 'BNE', 'BLT', 'BGE', 'BLTU', 'BGEU']:
        imm = binary_value[0] + binary_value[24] + binary_value[1:7] + binary_value[20:24]
        instruction = RISCVInstruction(hex_value, operation, None, f'x{int(rs1, 2)}', f'x{int(rs2, 2)}', hex(int(imm, 2)))
    # Jump instructions
    elif operation in ['JAL']:
        imm = binary_value[0] + binary_value[12:20] + binary_value[11] + binary_value[1:11]
        instruction = RISCVInstruction(hex_value, operation, f'x{int(rd, 2)}', None, None, hex(int(imm, 2)))
    elif operation in ['JALR']:
        imm = binary_value[0:12]
        instruction = RISCVInstruction(hex_value, operation, f'x{int(rd, 2)}', f'x{int(rs1, 2)}', None, hex(int(imm, 2)))
    # U-type instructions
    elif operation in ['LUI', 'AUIPC']:
        imm = binary_value[0:20]
        instruction = RISCVInstruction(hex_value, operation, f'x{int(rd, 2)}', None, None, hex(int(imm, 2)))
    # M-extension instructions
    elif operation in ['MUL', 'MULH', 'MULHSU', 'MULHU', 'DIV', 'DIVU', 'REM', 'REMU']:
        instruction = RISCVInstruction(hex_value, operation, f'x{int(rd, 2)}', f'x{int(rs1, 2)}', f'x{int(rs2, 2)}', None)
    else:
        return None

    return instruction


if __name__ == '__main__':
    print('This file is a module and not intended to be run directly.')
