import json
import urllib.request
import os

OLLAMA_URL = "http://localhost:11434/api/generate"
MODEL_NAME = "qwen2.5-coder:14b"

PROMPT = """You are a senior MIPS Allegrex (PSP) assembly engineer.
We are building a monolithic direct-threaded recompiler core for the Super FX 2 (GSU-2) chip in Snes9xTYL.
The emulator context is loaded into registers:
- $s0: Base pointer to struct FxRegs_s GSU
- $s1: nInstructions countdown cycle counter
- $s2: R15 (Super FX Program Counter)
- $s3: pvPrgBank pointer (ROM bank currently executing)
- $s4: pvSreg pointer (uint32* to source register)
- $s5: pvDreg pointer (uint32* to destination register)
- $s6: vStatusReg (GSU status register)

STRUCT OFFSETS in GSU ($s0):
- GSU_R0_OFF to GSU_R15_OFF: 0, 4, 8, ..., 60 (R0-R15 are uint32 each)
- GSU_VSIGN_OFF: 116
- GSU_VZERO_OFF: 120
- GSU_VCARRY_OFF: 124
- GSU_VOVERFLOW_OFF: 128

RULES & CONSTRAINTS:
1. Every opcode handler MUST end with:
   j .Lfx_dispatch_loop
   nop (or delay slot instruction)
2. Every branch/jump has a branch delay slot!
3. Do NOT call any functions with jal or stack frame. All work is done inline with registers $t0-$t9, $a0-$a3, $v0-$v1.
4. CLRFLAGS behavior: andi $s6, $s6, 0xEFFF (clears ALT1, ALT2, B flags). pvDreg and pvSreg reset to &R0 (addiu $s4, $s0, GSU_R0_OFF; addiu $s5, $s0, GSU_R0_OFF).
5. For arithmetic:
   - SREG is *($s4)
   - Result is written to *($s5)
   - Update GSU.vSign ($t_res), GSU.vZero ($t_res), GSU.vCarry (1 or 0), GSU.vOverflow
   - Overflow formula: ~(SREG ^ Rn) & (Rn ^ res) & 0x8000
   - Carry formula: (res >= 0x10000) ? 1 : 0

Please generate MIPS assembly handlers for:
1. .Lop_add_rn (0x50-0x5F ALT0: ADD Rn -> res = (SREG & 0xFFFF) + (Rn & 0xFFFF))
2. .Lop_adc_rn (0x50-0x5F ALT1: ADC Rn -> res = (SREG & 0xFFFF) + (Rn & 0xFFFF) + (vCarry & 1))
3. .Lop_sub_rn (0x60-0x6F ALT0: SUB Rn -> res = (SREG & 0xFFFF) - (Rn & 0xFFFF))
4. .Lop_sbc_rn (0x60-0x6F ALT1: SBC Rn -> res = (SREG & 0xFFFF) - (Rn & 0xFFFF) - (1 - (vCarry & 1)))
5. .Lop_cmp_rn (0x60-0x6F ALT3: CMP Rn -> like SUB but does not store result to DREG, only sets flags)
6. .Lop_and_rn (0x71-0x7F ALT0: AND Rn -> res = SREG & Rn)
7. .Lop_bic_rn (0x71-0x7F ALT1: BIC Rn -> res = SREG & ~Rn)
8. .Lop_or_rn  (0xC1-0xCF ALT0: OR Rn  -> res = SREG | Rn)
9. .Lop_xor_rn (0xC1-0xCF ALT1: XOR Rn -> res = SREG ^ Rn)
10. .Lop_inc_rn (0xD0-0xDE: INC Rn -> Rn++, update vSign, vZero, CLRFLAGS)
11. .Lop_dec_rn (0xE0-0xEE: DEC Rn -> Rn--, update vSign, vZero, CLRFLAGS)

Note: For Rn handlers, the opcode itself encodes n in lower 4 bits (e.g. opcode & 0x0F). Assume $t3 still holds the raw opcode from the fetch! So n = $t3 & 0x0F, and address of Rn is $s0 + (n << 2).

Output ONLY the raw MIPS assembly code inside a code block.
"""

def main():
    print("Calling local Ollama (qwen2.5-coder:14b) on RTX 4070...")
    req_data = {
        "model": MODEL_NAME,
        "prompt": PROMPT,
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
            
            # Extract code block
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
                
            output_path = os.path.join("psp", "sfx_mips", "fx_mips_arith.inc")
            with open(output_path, "w") as f:
                f.write("/* Auto-generated Monolithic Arithmetic Handlers (Phase 2B) */\n\n")
                f.write("\n".join(code_lines))
                f.write("\n")
                
            print(f"Successfully generated {output_path} ({len(code_lines)} lines)")
    except Exception as e:
        print(f"Error calling Ollama: {e}")

if __name__ == "__main__":
    main()
