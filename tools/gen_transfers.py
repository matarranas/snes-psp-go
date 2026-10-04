import urllib.request
import json
import os

OLLAMA_URL = "http://localhost:11434/api/generate"
MODEL_NAME = "qwen2.5-coder:14b"

PROMPT = """You are a senior MIPS Allegrex (PSP) assembly engineer.
We are continuing building the monolithic direct-threaded recompiler core for Super FX 2 (GSU-2) chip in Snes9xTYL.
CPU context registers:
- $s0: Base pointer to struct FxRegs_s GSU
- $s1: nInstructions countdown cycle counter
- $s2: R15 (Super FX Program Counter)
- $s3: pvPrgBank pointer (ROM bank currently executing)
- $s4: pvSreg pointer (uint32* to source register)
- $s5: pvDreg pointer (uint32* to destination register)
- $s6: vStatusReg (GSU status register)

STRUCT OFFSETS in GSU ($s0):
- GSU_R0_OFF .. GSU_R15_OFF: 0, 4, 8, ..., 60 (R0-R15 uint32 each)
- GSU_ROMBUF_OFF: 108
- GSU_ROMBANK_PTR: 468
- GSU_VSIGN_OFF: 116
- GSU_VZERO_OFF: 120
- GSU_VOVERFLOW_OFF: 128

RULES:
1. Every opcode handler MUST end with:
   j .Lfx_dispatch_loop
   nop (or delay slot instruction)
2. Every branch/jump has a branch delay slot!
3. All work inline with registers $t0-$t9, $a0-$a3, $v0-$v1. Do NOT touch $s0-$s7.
4. $t3 holds the current opcode byte!
   For Rn groups, n = $t3 & 0x0F, and address of Rn in GSU is $s0 + (n << 2).
5. READR14: When Rn is R14 (n == 14) and TESTR14/READR14 is triggered, load ROM(R14) into GSU.vRomBuffer:
   lw $ta, GSU_R14_OFF($s0); andi $ta, $ta, 0xFFFF; lw $tb, GSU_ROMBANK_PTR($s0); addu $tb, $tb, $ta; lbu $tc, 0($tb); sb $tc, GSU_ROMBUF_OFF($s0).

OPCODES TO GENERATE:
1. .Lop_to_rn (0x10-0x1F):
   Check B flag in $s6 (bit 12, 0x1000):
   If B set:
     lw $t1, 0($s4) /* SREG */
     sw $t1, 0($t_rn) /* Rn = SREG */
     if n == 14, READR14
     andi $s6, $s6, 0xEFFF /* CLRFLAGS */
     addu $s4, $s0, $zero
     addu $s5, $s0, $zero
   Else (B clear):
     move $s5, $t_rn /* pvDreg = &Rn */
   j .Lfx_dispatch_loop

2. .Lop_with_rn (0x20-0x2F):
   ori $s6, $s6, 0x1000 /* Set B flag */
   move $s4, $t_rn /* pvSreg = &Rn */
   move $s5, $t_rn /* pvDreg = &Rn */
   j .Lfx_dispatch_loop

3. .Lop_from_rn (0xB0-0xBF):
   Check B flag in $s6 (bit 12, 0x1000):
   If B set:
     lw $t1, 0($t_rn) /* v = Rn */
     sw $t1, 0($s5) /* *DREG = v */
     sw $t1, GSU_VSIGN_OFF($s0)
     sw $t1, GSU_VZERO_OFF($s0)
     andi $t2, $t1, 0x80
     sll $t2, $t2, 16
     sw $t2, GSU_VOVERFLOW_OFF($s0)
     if pvDreg == &R14, READR14
     andi $s6, $s6, 0xEFFF /* CLRFLAGS */
     addu $s4, $s0, $zero
     addu $s5, $s0, $zero
   Else (B clear):
     move $s4, $t_rn /* pvSreg = &Rn */
   j .Lfx_dispatch_loop

4. .Lop_ibt_rn (0xA0-0xAF):
   Read next byte from ROM at R15:
   andi $t1, $s2, 0xFFFF; addu $t2, $s3, $t1; lb $t1, 0($t2) /* sign extended */
   addiu $s2, $s2, 1 /* R15++ */
   sw $t1, 0($t_rn) /* Rn = SEX8(v) */
   andi $s6, $s6, 0xEFFF /* CLRFLAGS */
   addu $s4, $s0, $zero
   addu $s5, $s0, $zero
   j .Lfx_dispatch_loop

5. .Lop_iwt_rn (0xF0-0xFF):
   Read 16-bit word (little endian) from ROM at R15:
   andi $t1, $s2, 0xFFFF; addu $t2, $s3, $t1; lbu $t4, 0($t2)
   addiu $s2, $s2, 1
   andi $t1, $s2, 0xFFFF; addu $t2, $s3, $t1; lbu $t5, 0($t2)
   addiu $s2, $s2, 1
   sll $t5, $t5, 8; or $t4, $t4, $t5
   sw $t4, 0($t_rn) /* Rn = v */
   if n == 14, READR14
   andi $s6, $s6, 0xEFFF /* CLRFLAGS */
   addu $s4, $s0, $zero
   addu $s5, $s0, $zero
   j .Lfx_dispatch_loop

Output ONLY the raw MIPS assembly inside a markdown code block.
"""

def main():
    print("Calling local Ollama (qwen2.5-coder:14b) for Register Transfer Group...")
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
                
            out_file = os.path.join("psp", "sfx_mips", "fx_mips_transfers.inc")
            with open(out_file, "w") as f:
                f.write("/* Auto-generated Register Transfer Handlers (TO, WITH, FROM, IBT, IWT) */\n\n")
                f.write("\n".join(code_lines))
                f.write("\n")
            print(f"Generated {out_file} ({len(code_lines)} lines)")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
