import urllib.request
import json
import re
import os
import sys
import time

OLLAMA_URL = "http://localhost:11434/api/generate"
MODEL_NAME = "qwen2.5-coder:14b"
OUTPUT_DIR = "psp/sfx_mips"

# Complete opcode definitions for Super FX 2
ALL_OPCODES = [
    # Basic / Flow control
    {"name": "fx_stop", "c_code": "static inline void fx_stop() { GSU.vCounter = 0; GSU.vPlotOptionReg = 0; GSU.vPipe = 1; CLRFLAGS; R15++; }"},
    {"name": "fx_nop", "c_code": "static inline void fx_nop() { CLRFLAGS; R15++; }"},
    {"name": "fx_cache", "c_code": "static inline void fx_cache() { uint32 c = R15 & 0xfff0; GSU.vCacheBaseReg = c; GSU.bCacheActive = TRUE; R15++; CLRFLAGS; }"},
    {"name": "fx_lsr", "c_code": "static inline void fx_lsr() { uint32 v; GSU.vCarry = SREG & 1; v = (SREG & 0xffff) >> 1; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_rol", "c_code": "static inline void fx_rol() { uint32 v = ((SREG << 1) + GSU.vCarry) & 0xffff; GSU.vCarry = (SREG >> 15) & 1; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_bra", "c_code": "static inline void fx_bra() { uint8 v = PIPE; R15++; FETCHPIPE; R15 += (int32)(int8)v; }"},
    {"name": "fx_bne", "c_code": "static inline void fx_bne() { uint8 v = PIPE; R15++; FETCHPIPE; if ((GSU.vZero & 0xffff) != 0) R15 += (int32)(int8)v; else R15++; }"},
    {"name": "fx_beq", "c_code": "static inline void fx_beq() { uint8 v = PIPE; R15++; FETCHPIPE; if ((GSU.vZero & 0xffff) == 0) R15 += (int32)(int8)v; else R15++; }"},
    {"name": "fx_bpl", "c_code": "static inline void fx_bpl() { uint8 v = PIPE; R15++; FETCHPIPE; if (!(GSU.vSign & 0x8000)) R15 += (int32)(int8)v; else R15++; }"},
    {"name": "fx_bmi", "c_code": "static inline void fx_bmi() { uint8 v = PIPE; R15++; FETCHPIPE; if (GSU.vSign & 0x8000) R15 += (int32)(int8)v; else R15++; }"},
    {"name": "fx_bcc", "c_code": "static inline void fx_bcc() { uint8 v = PIPE; R15++; FETCHPIPE; if (!(GSU.vCarry & 1)) R15 += (int32)(int8)v; else R15++; }"},
    {"name": "fx_bcs", "c_code": "static inline void fx_bcs() { uint8 v = PIPE; R15++; FETCHPIPE; if (GSU.vCarry & 1) R15 += (int32)(int8)v; else R15++; }"},

    # Modes ALT1, ALT2, ALT3, LOOP
    {"name": "fx_loop", "c_code": "static inline void fx_loop() { GSU.vSign = GSU.vZero = --R12; if ((uint16)R12 != 0) R15 = R13; else R15++; CLRFLAGS; }"},
    {"name": "fx_alt1", "c_code": "static inline void fx_alt1() { SF(ALT1); CF(B); R15++; }"},
    {"name": "fx_alt2", "c_code": "static inline void fx_alt2() { SF(ALT2); CF(B); R15++; }"},
    {"name": "fx_alt3", "c_code": "static inline void fx_alt3() { SF(ALT1); SF(ALT2); CF(B); R15++; }"},

    # Register transfers and bitwise operations
    {"name": "fx_swap", "c_code": "static inline void fx_swap() { uint8 c = (uint8)SREG; uint8 d = (uint8)(SREG >> 8); uint32 v = (((uint32)c) << 8) | ((uint32)d); R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_not", "c_code": "static inline void fx_not() { uint32 v = ~SREG & 0xffff; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_merge", "c_code": "static inline void fx_merge() { uint32 v = (R7 & 0xff00) | ((R8 & 0xff00) >> 8); R15++; DREG = v; GSU.vZero = !(v & 0xf0f0); GSU.vSign = ((v | (v << 8)) & 0x8000); GSU.vCarry = (v & 0xe0e0) != 0; CLRFLAGS; }"},
    {"name": "fx_sex", "c_code": "static inline void fx_sex() { uint32 v = (uint32)(int32)(int8)SREG; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_asr", "c_code": "static inline void fx_asr() { uint32 v; GSU.vCarry = SREG & 1; v = (uint32)((int32)(int16)SREG >> 1) & 0xffff; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_ror", "c_code": "static inline void fx_ror() { uint32 v = ((SREG & 0xffff) >> 1) | ((GSU.vCarry & 1) << 15); GSU.vCarry = SREG & 1; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_lob", "c_code": "static inline void fx_lob() { uint32 v = SREG & 0xff; R15++; DREG = v; GSU.vSign = v << 8; GSU.vZero = v << 8; CLRFLAGS; }"},
    {"name": "fx_hib", "c_code": "static inline void fx_hib() { uint32 v = (SREG >> 8) & 0xff; R15++; DREG = v; GSU.vSign = v << 8; GSU.vZero = v << 8; CLRFLAGS; }"},

    # Arithmetic and Logic core (Register-to-Register)
    {"name": "fx_add", "c_code": "static inline void fx_add(int reg) { int32 s = (int32)(int16)SREG + (int32)(int16)GSU.avReg[reg]; GSU.vCarry = s >= 0x10000; GSU.vOverflow = ~(SREG ^ GSU.avReg[reg]) & (GSU.avReg[reg] ^ s) & 0x8000; GSU.vSign = s; GSU.vZero = s; R15++; DREG = s & 0xffff; CLRFLAGS; }"},
    {"name": "fx_adc", "c_code": "static inline void fx_adc(int reg) { int32 s = (int32)(int16)SREG + (int32)(int16)GSU.avReg[reg] + (GSU.vCarry & 1); GSU.vCarry = s >= 0x10000; GSU.vOverflow = ~(SREG ^ GSU.avReg[reg]) & (GSU.avReg[reg] ^ s) & 0x8000; GSU.vSign = s; GSU.vZero = s; R15++; DREG = s & 0xffff; CLRFLAGS; }"},
    {"name": "fx_sub", "c_code": "static inline void fx_sub(int reg) { int32 s = (int32)(int16)SREG - (int32)(int16)GSU.avReg[reg]; GSU.vCarry = s >= 0; GSU.vOverflow = (SREG ^ GSU.avReg[reg]) & (SREG ^ s) & 0x8000; GSU.vSign = s; GSU.vZero = s; R15++; DREG = s & 0xffff; CLRFLAGS; }"},
    {"name": "fx_sbc", "c_code": "static inline void fx_sbc(int reg) { int32 s = (int32)(int16)SREG - (int32)(int16)GSU.avReg[reg] - ((GSU.vCarry & 1) ^ 1); GSU.vCarry = s >= 0; GSU.vOverflow = (SREG ^ GSU.avReg[reg]) & (SREG ^ s) & 0x8000; GSU.vSign = s; GSU.vZero = s; R15++; DREG = s & 0xffff; CLRFLAGS; }"},
    {"name": "fx_cmp", "c_code": "static inline void fx_cmp(int reg) { int32 s = (int32)(int16)SREG - (int32)(int16)GSU.avReg[reg]; GSU.vCarry = s >= 0; GSU.vOverflow = (SREG ^ GSU.avReg[reg]) & (SREG ^ s) & 0x8000; GSU.vSign = s; GSU.vZero = s; R15++; CLRFLAGS; }"},
    {"name": "fx_and", "c_code": "static inline void fx_and(int reg) { uint32 v = SREG & GSU.avReg[reg]; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_bic", "c_code": "static inline void fx_bic(int reg) { uint32 v = SREG & ~GSU.avReg[reg]; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_or", "c_code": "static inline void fx_or(int reg) { uint32 v = (SREG | GSU.avReg[reg]) & 0xffff; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_xor", "c_code": "static inline void fx_xor(int reg) { uint32 v = (SREG ^ GSU.avReg[reg]) & 0xffff; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},

    # Multiplication
    {"name": "fx_mult", "c_code": "static inline void fx_mult(int reg) { uint32 v = (uint32)((int32)(int8)SREG * (int32)(int8)GSU.avReg[reg]) & 0xffff; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_umult", "c_code": "static inline void fx_umult(int reg) { uint32 v = ((SREG & 0xff) * (GSU.avReg[reg] & 0xff)) & 0xffff; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; CLRFLAGS; }"},
    {"name": "fx_fmult", "c_code": "static inline void fx_fmult() { uint32 c = (uint32)((int32)(int16)SREG * (int32)(int16)R6); uint32 v = (c >> 16) & 0xffff; R15++; DREG = v; GSU.vSign = v; GSU.vZero = v; GSU.vCarry = (c >> 15) & 1; CLRFLAGS; }"}
]

PROMPT_TEMPLATE = """You are a senior compiler and MIPS assembly engineer optimizing Super Nintendo Super FX 2 (GSU-2) emulation for Sony PSP (Allegrex CPU).

Target architecture:
- MIPS Allegrex (MIPS32 Release 2 compatible, 32-bit registers, little-endian).
- Assembler: GNU as (psp-gcc / psp-as).
- Syntax: AT&T / Standard MIPS syntax (.set noreorder, etc).

Memory Layout (Pointer to struct FxRegs_s GSU is passed in register $a0):
- R0..R15 are uint32 at offset 0..60 ($a0 + reg_idx * 4). (R15 is PC).
- pvDreg is pointer at offset 100 ($a0).
- pvSreg is pointer at offset 104 ($a0).
- vSign is uint32 at offset 116 ($a0).
- vZero is uint32 at offset 120 ($a0).
- vCarry is uint32 at offset 124 ($a0).
- vOverflow is int32 at offset 128 ($a0).

Register usage convention for this leaf function:
- $a0: Pointer to GSU struct (MUST NOT BE CLOBBERED or must be preserved).
- $a1: Register index argument (if function takes 'reg').
- $v0, $v1: Temporary / return values.
- $t0..$t9: Temporary scratchpad registers (caller-saved).
- $ra: Return address. Always end with:
    jr $ra
    (instruction in branch delay slot, or nop if none)

Task:
Convert the following C function into high-performance GNU MIPS assembly:

```c
{c_code}
```

Requirements:
1. Provide ONLY valid assembly code enclosed in ```asm ... ``` block.
2. Optimize for Allegrex (avoid pipeline stalls, fill branch delay slots when safe).
3. Do not invent symbols; use correct offsets.
"""

def query_ollama(prompt):
    payload = {
        "model": MODEL_NAME,
        "prompt": prompt,
        "stream": False,
        "options": {
            "temperature": 0.1,
            "top_p": 0.9
        }
    }
    req = urllib.request.Request(
        OLLAMA_URL,
        data=json.dumps(payload).encode("utf-8"),
        headers={"Content-Type": "application/json"}
    )
    with urllib.request.urlopen(req, timeout=180) as resp:
        res = json.loads(resp.read().decode("utf-8"))
        return res.get("response", "")

def extract_asm(text):
    match = re.search(r"```(?:asm|assembly)?(.*?)```", text, re.DOTALL | re.IGNORECASE)
    if match:
        return match.group(1).strip()
    return text.strip()

def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    total = len(ALL_OPCODES)
    print(f"=== Super FX 2 MIPS Transpiler (Ollama {MODEL_NAME}) ===")
    print(f"Total opcodes to transpile: {total}")
    print(f"Output directory: {OUTPUT_DIR}\n")

    start_time = time.time()
    for idx, item in enumerate(ALL_OPCODES, 1):
        name = item["name"]
        out_file = os.path.join(OUTPUT_DIR, f"{name}.S")
        if os.path.exists(out_file) and os.path.getsize(out_file) > 100:
            print(f"[{idx}/{total}] Skipping {name} (already exists)")
            continue

        print(f"[{idx}/{total}] Transpiling {name}...")
        prompt = PROMPT_TEMPLATE.format(c_code=item["c_code"])
        try:
            raw_resp = query_ollama(prompt)
            asm_code = extract_asm(raw_resp)
            with open(out_file, "w", encoding="utf-8") as f:
                f.write(f"/* Auto-generated MIPS Allegrex for {name} */\n")
                f.write(".set noreorder\n")
                f.write(".text\n")
                f.write(f".globl mips_{name}\n")
                f.write(f"mips_{name}:\n")
                f.write(asm_code + "\n")
            print(f"    -> OK ({len(asm_code.splitlines())} lines)")
        except Exception as e:
            print(f"    -> ERROR on {name}: {e}")

    elapsed = time.time() - start_time
    print(f"\nAll opcodes processed in {elapsed:.1f} seconds!")

if __name__ == "__main__":
    main()
