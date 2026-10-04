import urllib.request
import json
import re
import os
import sys
import time

OLLAMA_URL = "http://localhost:11434/api/generate"
MODEL_NAME = "qwen2.5-coder:14b"
CORE_FILE = "psp/sfx_mips/fx_mips_core.S"
OUT_HANDLERS_FILE = "psp/sfx_mips/fx_mips_handlers.inc"

# Phase 2 Opcode Handlers for Monolithic Direct Threaded Dispatch
# In this architecture:
# - $s0: GSU base pointer (callee-saved)
# - $s1: nInstructions counter
# - $s2: R15 (PC)
# - $s3: pvPrgBank pointer
# - $s4: pvSreg pointer (points to current source reg)
# - $s5: pvDreg pointer (points to current dest reg)
# - $s6: vStatusReg
# - All handlers end with:
#     j .Lfx_dispatch_loop
#     nop (or delay slot instruction)

PHASE2_OPCODES = [
    {
        "name": "Lop_lsr",
        "opcode_hex": "0x03",
        "description": "LSR: Logical shift right SREG by 1, store in DREG, set Carry, Sign, Zero, CLRFLAGS",
        "c_code": """
        uint32 v;
        GSU.vCarry = SREG & 1;
        v = (SREG & 0xffff) >> 1;
        DREG = v;
        GSU.vSign = v;
        GSU.vZero = v;
        CLRFLAGS;
        """
    },
    {
        "name": "Lop_rol",
        "opcode_hex": "0x04",
        "description": "ROL: Rotate left SREG with Carry, store in DREG, update Carry, Sign, Zero, CLRFLAGS",
        "c_code": """
        uint32 v = ((SREG << 1) + GSU.vCarry) & 0xffff;
        GSU.vCarry = (SREG >> 15) & 1;
        DREG = v;
        GSU.vSign = v;
        GSU.vZero = v;
        CLRFLAGS;
        """
    },
    {
        "name": "Lop_bra",
        "opcode_hex": "0x05",
        "description": "BRA: Branch always (read 8-bit signed offset from ROM at R15, R15 += offset)",
        "c_code": """
        andi $t1, $s2, 0xffff;
        addu $t2, $s3, $t1;
        lb $t0, 0($t2);
        addiu $s2, $s2, 1;
        addu $s2, $s2, $t0;
        """
    },
    {
        "name": "Lop_bne",
        "opcode_hex": "0x08",
        "description": "BNE: Branch if Zero flag is clear",
        "c_code": """
        lw $t4, GSU_VZERO_OFF($s0);
        andi $t1, $s2, 0xffff;
        addu $t2, $s3, $t1;
        lb $t0, 0($t2);
        addiu $s2, $s2, 1;
        andi $t4, $t4, 0xffff;
        bnez $t4, 1f;
        nop;
        j .Lfx_dispatch_loop;
        nop;
        1: addu $s2, $s2, $t0;
        """
    },
    {
        "name": "Lop_beq",
        "opcode_hex": "0x09",
        "description": "BEQ: Branch if Zero flag is set",
        "c_code": """
        lw $t4, GSU_VZERO_OFF($s0);
        andi $t1, $s2, 0xffff;
        addu $t2, $s3, $t1;
        lb $t0, 0($t2);
        addiu $s2, $s2, 1;
        andi $t4, $t4, 0xffff;
        beqz $t4, 1f;
        nop;
        j .Lfx_dispatch_loop;
        nop;
        1: addu $s2, $s2, $t0;
        """
    },
    {
        "name": "Lop_bpl",
        "opcode_hex": "0x0A",
        "description": "BPL: Branch if Sign flag is clear (positive)",
        "c_code": """
        lw $t4, GSU_VSIGN_OFF($s0);
        andi $t1, $s2, 0xffff;
        addu $t2, $s3, $t1;
        lb $t0, 0($t2);
        addiu $s2, $s2, 1;
        andi $t4, $t4, 0x8000;
        beqz $t4, 1f;
        nop;
        j .Lfx_dispatch_loop;
        nop;
        1: addu $s2, $s2, $t0;
        """
    },
    {
        "name": "Lop_bmi",
        "opcode_hex": "0x0B",
        "description": "BMI: Branch if Sign flag is set (negative)",
        "c_code": """
        lw $t4, GSU_VSIGN_OFF($s0);
        andi $t1, $s2, 0xffff;
        addu $t2, $s3, $t1;
        lb $t0, 0($t2);
        addiu $s2, $s2, 1;
        andi $t4, $t4, 0x8000;
        bnez $t4, 1f;
        nop;
        j .Lfx_dispatch_loop;
        nop;
        1: addu $s2, $s2, $t0;
        """
    },
    {
        "name": "Lop_bcc",
        "opcode_hex": "0x0C",
        "description": "BCC: Branch if Carry flag is clear",
        "c_code": """
        lw $t4, GSU_VCARRY_OFF($s0);
        andi $t1, $s2, 0xffff;
        addu $t2, $s3, $t1;
        lb $t0, 0($t2);
        addiu $s2, $s2, 1;
        andi $t4, $t4, 1;
        beqz $t4, 1f;
        nop;
        j .Lfx_dispatch_loop;
        nop;
        1: addu $s2, $s2, $t0;
        """
    },
    {
        "name": "Lop_bcs",
        "opcode_hex": "0x0D",
        "description": "BCS: Branch if Carry flag is set",
        "c_code": """
        lw $t4, GSU_VCARRY_OFF($s0);
        andi $t1, $s2, 0xffff;
        addu $t2, $s3, $t1;
        lb $t0, 0($t2);
        addiu $s2, $s2, 1;
        andi $t4, $t4, 1;
        bnez $t4, 1f;
        nop;
        j .Lfx_dispatch_loop;
        nop;
        1: addu $s2, $s2, $t0;
        """
    },
    {
        "name": "Lop_loop",
        "opcode_hex": "0x3C",
        "description": "LOOP: Decrement R12; if R12 != 0 then R15 = R13 else R15++; CLRFLAGS",
        "c_code": """
        lw $t0, GSU_R12_OFF($s0);
        subiu $t0, $t0, 1;
        sw $t0, GSU_R12_OFF($s0);
        sw $t0, GSU_VSIGN_OFF($s0);
        sw $t0, GSU_VZERO_OFF($s0);
        andi $t1, $t0, 0xffff;
        bnez $t1, 1f;
        nop;
        addiu $s2, $s2, 1;
        j 2f;
        nop;
        1: lw $s2, GSU_R13_OFF($s0);
        2: andi $s6, $s6, 0xEFFF;
        """
    },
    {
        "name": "Lop_alt1",
        "opcode_hex": "0x3D",
        "description": "ALT1: Set ALT1 flag (bit 8), clear B flag (bit 12) in vStatusReg",
        "c_code": """
        ori $s6, $s6, 0x0100;
        andi $s6, $s6, 0xEFFF;
        """
    },
    {
        "name": "Lop_alt2",
        "opcode_hex": "0x3E",
        "description": "ALT2: Set ALT2 flag (bit 9), clear B flag (bit 12) in vStatusReg",
        "c_code": """
        ori $s6, $s6, 0x0200;
        andi $s6, $s6, 0xEFFF;
        """
    },
    {
        "name": "Lop_alt3",
        "opcode_hex": "0x3F",
        "description": "ALT3: Set ALT1 and ALT2 flags (bits 8 and 9), clear B flag (bit 12) in vStatusReg",
        "c_code": """
        ori $s6, $s6, 0x0300;
        andi $s6, $s6, 0xEFFF;
        """
    },
    {
        "name": "Lop_asr",
        "opcode_hex": "0x96",
        "description": "ASR: Arithmetic shift right SREG by 1, update Carry, Sign, Zero, CLRFLAGS",
        "c_code": """
        lw $t0, 0($s4);
        andi $t1, $t0, 1;
        sw $t1, GSU_VCARRY_OFF($s0);
        seh $t2, $t0;
        sra $t2, $t2, 1;
        andi $t2, $t2, 0xffff;
        sw $t2, 0($s5);
        sw $t2, GSU_VSIGN_OFF($s0);
        sw $t2, GSU_VZERO_OFF($s0);
        andi $s6, $s6, 0xEFFF;
        """
    },
    {
        "name": "Lop_ror",
        "opcode_hex": "0x97",
        "description": "ROR: Rotate right SREG with Carry, update Carry, Sign, Zero, CLRFLAGS",
        "c_code": """
        lw $t0, 0($s4);
        lw $t1, GSU_VCARRY_OFF($s0);
        andi $t2, $t0, 1;
        andi $t3, $t0, 0xffff;
        srl $t3, $t3, 1;
        sll $t4, $t1, 15;
        or $t3, $t3, $t4;
        andi $t3, $t3, 0xffff;
        sw $t2, GSU_VCARRY_OFF($s0);
        sw $t3, 0($s5);
        sw $t3, GSU_VSIGN_OFF($s0);
        sw $t3, GSU_VZERO_OFF($s0);
        andi $s6, $s6, 0xEFFF;
        """
    },
    {
        "name": "Lop_lob",
        "opcode_hex": "0x9E",
        "description": "LOB: Mask SREG lower 8 bits, store DREG, Sign/Zero = v<<8, CLRFLAGS",
        "c_code": """
        lw $t0, 0($s4);
        andi $t1, $t0, 0x00FF;
        sw $t1, 0($s5);
        sll $t2, $t1, 8;
        sw $t2, GSU_VSIGN_OFF($s0);
        sw $t2, GSU_VZERO_OFF($s0);
        andi $s6, $s6, 0xEFFF;
        """
    },
    {
        "name": "Lop_hib",
        "opcode_hex": "0xC0",
        "description": "HIB: SREG upper 8 bits to lower 8 bits, store DREG, Sign/Zero = v<<8, CLRFLAGS",
        "c_code": """
        lw $t0, 0($s4);
        srl $t1, $t0, 8;
        andi $t1, $t1, 0x00FF;
        sw $t1, 0($s5);
        sll $t2, $t1, 8;
        sw $t2, GSU_VSIGN_OFF($s0);
        sw $t2, GSU_VZERO_OFF($s0);
        andi $s6, $s6, 0xEFFF;
        """
    }
]

PROMPT_TEMPLATE = """You are a senior MIPS Allegrex assembly optimization expert for Sony PSP.
Target: Monolithic handler block inside `fx_mips_core.S`.
Rules:
- DO NOT generate a function prologue or epilogue (no `addiu $sp`, no `jr $ra`).
- Register mappings in this monolithic loop:
    $s0: Base pointer to struct FxRegs_s GSU
    $s1: nInstructions cycle counter
    $s2: R15 (Program Counter)
    $s3: pvPrgBank pointer
    $s4: pvSreg pointer
    $s5: pvDreg pointer
    $s6: vStatusReg
- End the block with:
    j .Lfx_dispatch_loop
    nop (or instruction in delay slot)
- Include handler label `.{handler_name}:`
- Code logic to translate:
{c_code}

Return ONLY the assembly code block inside ```asm ... ```.
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
    print(f"=== Phase 2: Monolithic Direct Threaded Handler Generator ===")
    print(f"Model: {MODEL_NAME} on RTX 4070")
    print(f"Total handlers to generate: {len(PHASE2_OPCODES)}\n")

    generated_blocks = []
    start_time = time.time()

    for idx, item in enumerate(PHASE2_OPCODES, 1):
        name = item["name"]
        print(f"[{idx}/{len(PHASE2_OPCODES)}] Generating {name} ({item['opcode_hex']})...")
        prompt = PROMPT_TEMPLATE.format(handler_name=name, c_code=item["c_code"])
        try:
            resp = query_ollama(prompt)
            asm_code = extract_asm(resp)
            generated_blocks.append(f"/* {item['opcode_hex']}: {item['description']} */\n.{name}:\n" + asm_code + "\n")
            print(f"    -> OK ({len(asm_code.splitlines())} lines)")
        except Exception as e:
            print(f"    -> ERROR: {e}")

    with open(OUT_HANDLERS_FILE, "w", encoding="utf-8") as f:
        f.write("/* Auto-generated Monolithic Phase 2 Handlers */\n\n")
        f.write("\n".join(generated_blocks))

    elapsed = time.time() - start_time
    print(f"\nPhase 2 generation complete in {elapsed:.1f}s! Saved to {OUT_HANDLERS_FILE}")

if __name__ == "__main__":
    main()
