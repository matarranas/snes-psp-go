/* Auto-generated MIPS Allegrex for fx_sbc */
.set noreorder
.text
.globl mips_fx_sbc
mips_fx_sbc:
.set noreorder

fx_sbc:
    # Load SREG and GSU.avReg[reg] into $t0 and $t1
    lw $t0, 104($a0)          # Load pvSreg into $t0
    lw $t1, 0($a0)            # Load R0 (GSU.avReg[0]) into $t1
    sll $t2, $a1, 2           # Calculate offset for GSU.avReg[reg]
    addu $t1, $t1, $t2        # $t1 now points to GSU.avReg[reg]
    lw $t1, 0($t1)            # Load GSU.avReg[reg] into $t1

    # Load SREG value into $t3
    lw $t3, 0($t0)            # Load SREG into $t3

    # Sign-extend SREG and GSU.avReg[reg] to 32-bit
    seb $t3, $t3              # Sign-extend SREG to 32-bit
    seb $t1, $t1              # Sign-extend GSU.avReg[reg] to 32-bit

    # Load vCarry into $t2
    lw $t2, 124($a0)          # Load vCarry into $t2
    andi $t2, $t2, 1          # Mask vCarry to 1-bit
    xori $t2, $t2, 1          # Invert vCarry

    # Perform subtraction: s = SREG - GSU.avReg[reg] - vCarry
    subu $t4, $t3, $t1        # s = SREG - GSU.avReg[reg]
    subu $t4, $t4, $t2        # s = s - vCarry

    # Update vCarry
    slt $t2, $t4, $zero       # vCarry = s < 0
    sw $t2, 124($a0)          # Store vCarry

    # Update vOverflow
    xor $t5, $t3, $t1         # (SREG ^ GSU.avReg[reg])
    xor $t6, $t3, $t4         # (SREG ^ s)
    and $t5, $t5, $t6         # (SREG ^ GSU.avReg[reg]) & (SREG ^ s)
    andi $t5, $t5, 0x8000     # & 0x8000
    sw $t5, 128($a0)          # Store vOverflow

    # Update vSign
    sw $t4, 116($a0)          # Store s into vSign

    # Update vZero
    sw $t4, 120($a0)          # Store s into vZero

    # Increment R15 (PC)
    lw $t0, 60($a0)           # Load R15 (PC) into $t0
    addiu $t0, $t0, 1         # Increment R15
    sw $t0, 60($a0)           # Store incremented R15 back

    # Update DREG
    lw $t0, 100($a0)          # Load pvDreg into $t0
    andi $t4, $t4, 0xffff     # s & 0xffff
    sw $t4, 0($t0)            # Store s & 0xffff into DREG

    # Return
    jr $ra
    nop
