import os

# Table of 1024 opcodes: 0-255 ALT0, 256-511 ALT1, 512-767 ALT2, 768-1023 ALT3
table = [".Lop_default_fallback"] * 1024

for alt in range(4):
    base = alt * 256
    # 0x00: STOP
    table[base + 0x00] = ".Lop_stop"
    # 0x01: NOP
    table[base + 0x01] = ".Lop_nop"
    # 0x03: LSR
    table[base + 0x03] = ".Lop_lsr"
    # 0x04: ROL
    table[base + 0x04] = ".Lop_rol"
    # 0x05: BRA
    table[base + 0x05] = ".Lop_bra"
    # Branches 0x08-0x0D
    table[base + 0x08] = ".Lop_bne"
    table[base + 0x09] = ".Lop_beq"
    table[base + 0x0A] = ".Lop_bpl"
    table[base + 0x0B] = ".Lop_bmi"
    table[base + 0x0C] = ".Lop_bcc"
    table[base + 0x0D] = ".Lop_bcs"
    # 0x3C: LOOP
    table[base + 0x3C] = ".Lop_loop"
    # 0x3D: ALT1
    table[base + 0x3D] = ".Lop_alt1"
    # 0x3E: ALT2
    table[base + 0x3E] = ".Lop_alt2"
    # 0x3F: ALT3
    table[base + 0x3F] = ".Lop_alt3"
    # 0x4D: SWAP
    table[base + 0x4D] = ".Lop_swap"
    # 0x4F: NOT
    table[base + 0x4F] = ".Lop_not"
    # 0x95: SEX
    table[base + 0x95] = ".Lop_sex"
    # 0x96: ASR
    table[base + 0x96] = ".Lop_asr"
    # 0x97: ROR
    table[base + 0x97] = ".Lop_ror"
    # 0x9E: LOB
    table[base + 0x9E] = ".Lop_lob"
    # 0x9F: FMULT
    table[base + 0x9F] = ".Lop_fmult"
    # 0xC0: HIB
    table[base + 0xC0] = ".Lop_hib"
    # 0xD0 - 0xDE: INC
    for r in range(15):
        table[base + 0xD0 + r] = ".Lop_inc_rn"
    # 0xE0 - 0xEE: DEC
    for r in range(15):
        table[base + 0xE0 + r] = ".Lop_dec_rn"

# ALT0 specifics (alt = 0)
for r in range(16):
    table[0 * 256 + 0x50 + r] = ".Lop_add_rn"
    table[0 * 256 + 0x60 + r] = ".Lop_sub_rn"
for r in range(1, 16):
    table[0 * 256 + 0x70 + r] = ".Lop_and_rn"
    table[0 * 256 + 0xC0 + r] = ".Lop_or_rn"

# ALT1 specifics (alt = 1)
for r in range(16):
    table[1 * 256 + 0x50 + r] = ".Lop_adc_rn"
    table[1 * 256 + 0x60 + r] = ".Lop_sbc_rn"
for r in range(1, 16):
    table[1 * 256 + 0x70 + r] = ".Lop_bic_rn"
    table[1 * 256 + 0xC0 + r] = ".Lop_xor_rn"

# ALT3 specifics (alt = 3)
for r in range(16):
    table[3 * 256 + 0x60 + r] = ".Lop_cmp_rn"

# Format assembly table
asm_lines = [
    "/* Auto-generated 1024-entry Jump Table for ALT0..ALT3 */",
    ".data",
    ".align 4",
    ".globl fx_mips_jump_table",
    "fx_mips_jump_table:"
]

for i, handler in enumerate(table):
    alt_mode = i // 256
    opcode = i % 256
    asm_lines.append(f"    .word {handler} /* 0x{i:03X} (ALT{alt_mode}: 0x{opcode:02X}) */")

with open("psp/sfx_mips/fx_mips_table.inc", "w") as f:
    f.write("\n".join(asm_lines) + "\n")

print(f"Generated psp/sfx_mips/fx_mips_table.inc with {len(table)} entries.")
