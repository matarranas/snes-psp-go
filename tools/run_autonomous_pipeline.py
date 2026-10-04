import urllib.request
import json
import os
import sys

OLLAMA_URL = "http://localhost:11434/api/generate"
MODEL_NAME = "qwen2.5-coder:14b"

print("==================================================================")
print("  SUPER FX 2 RECOMPILER: LOCAL OLLAMA RTX 4070 PIPELINE")
print("==================================================================")

PROMPTS = [
    {
        "name": "Phase 3A: RAM Load & Store Handlers (LDW, LDB, STW, STB)",
        "file": "psp/sfx_mips/fx_mips_ram.inc",
        "prompt": """You are a senior MIPS Allegrex (PSP) assembly engineer.
Write monolithic assembly handlers for Super FX 2 RAM load/stores:
Context:
- $s0: GSU pointer
- $s2: R15 (PC)
- $s3: pvPrgBank
- $s4: pvSreg
- $s5: pvDreg
- $s6: vStatusReg
Offsets:
- GSU_RAMBANK_PTR: 464 (uint8* pvRamBank)
- GSU_LASTRAM_OFF: 96
- GSU_R14_OFF: 56
- GSU_ROMBUF_OFF: 108
- GSU_ROMBANK_PTR: 468

Opcodes to write:
1. .Lop_ldw_rn (0x40-0x4B ALT0: LDW (Rn)):
   n = opcode & 0x0F. Address = Rn & 0xFFFF.
   Load 16-bit word little endian from pvRamBank[Address].
   Store into DREG. Update vLastRamAdr. If pvDreg == &R14, READR14.
   CLRFLAGS: andi $s6, $s6, 0xEFFF; reset pvSreg/pvDreg to R0.
   j .Lfx_dispatch_loop.

2. .Lop_ldb_rn (0x40-0x4B ALT1: LDB (Rn)):
   Load 8-bit byte from pvRamBank[Address].
   Store into DREG. Update vLastRamAdr. If pvDreg == &R14, READR14.
   CLRFLAGS; j .Lfx_dispatch_loop.

3. .Lop_stw_rn (0x30-0x3B ALT0: STW (Rn)):
   Store 16-bit word little endian from SREG into pvRamBank[Address].
   Update vLastRamAdr. CLRFLAGS; j .Lfx_dispatch_loop.

4. .Lop_stb_rn (0x30-0x3B ALT1: STB (Rn)):
   Store 8-bit lower byte from SREG into pvRamBank[Address].
   Update vLastRamAdr. CLRFLAGS; j .Lfx_dispatch_loop.

Output ONLY MIPS assembly in a code block.
"""
    },
    {
        "name": "Phase 3B: ROM Fetch Handlers (GETC, GETB, GETBL, GETBH, GETBS)",
        "file": "psp/sfx_mips/fx_mips_rom.inc",
        "prompt": """You are a senior MIPS Allegrex (PSP) assembly engineer.
Write monolithic assembly handlers for Super FX 2 ROM access:
Context:
- $s0: GSU pointer
- $s4: pvSreg
- $s5: pvDreg
- $s6: vStatusReg
Offsets:
- GSU_COLOR_OFF: 64
- GSU_PLOTOPT_OFF: 68
- GSU_ROMBUF_OFF: 108
- GSU_ROMBANK_PTR: 468
- GSU_R14_OFF: 56
- GSU_VSIGN_OFF: 116
- GSU_VZERO_OFF: 120

Opcodes to write:
1. .Lop_getc (0xDF ALT0 / 0xDF ALT1):
   Load byte from GSU.vRomBuffer.
   If (vPlotOptionReg & 0x04): c = (c & 0xF0) | (c >> 4).
   If (vPlotOptionReg & 0x08): vColorReg = (vColorReg & 0xF0) | (c & 0x0F);
   Else: vColorReg = c.
   CLRFLAGS; j .Lfx_dispatch_loop.

2. .Lop_getb (0xEF ALT0):
   Load byte from GSU.vRomBuffer. Store in DREG.
   CLRFLAGS; j .Lfx_dispatch_loop.

3. .Lop_getbl (0xEF ALT2):
   Load byte from GSU.vRomBuffer.
   DREG = (DREG & 0xFF00) | (c & 0x00FF).
   CLRFLAGS; j .Lfx_dispatch_loop.

4. .Lop_getbh (0xEF ALT1):
   Load byte from GSU.vRomBuffer.
   DREG = (DREG & 0x00FF) | ((c & 0x00FF) << 8).
   CLRFLAGS; j .Lfx_dispatch_loop.

5. .Lop_getbs (0xEF ALT3):
   Load byte from GSU.vRomBuffer, sign-extend (SEB), store in DREG.
   CLRFLAGS; j .Lfx_dispatch_loop.

Output ONLY MIPS assembly in a code block.
"""
    }
]

for task in PROMPTS:
    print(f"\n[+] Processing {task['name']}...")
    req_data = {
        "model": MODEL_NAME,
        "prompt": task["prompt"],
        "stream": False
    }
    req = urllib.request.Request(
        OLLAMA_URL,
        data=json.dumps(req_data).encode('utf-8'),
        headers={'Content-Type': 'application/json'}
    )
    try:
        with urllib.request.urlopen(req, timeout=300) as response:
            result = json.loads(response.read().decode('utf-8'))
            response_text = result.get("response", "")
            lines = response_text.splitlines()
            code_lines = []
            in_code = False
            for line in lines:
                if line.strip().startswith("```"):
                    in_code = not in_code
                    continue
                if in_code:
                    code_lines.append(line)
            if not code_lines:
                code_lines = lines
            with open(task["file"], "w") as f:
                f.write(f"/* {task['name']} */\n\n")
                f.write("\n".join(code_lines) + "\n")
            print(f"  -> Saved {task['file']} ({len(code_lines)} lines)")
    except Exception as e:
        print(f"  -> Error: {e}")

print("\n[+] All autonomous local tasks completed!")
