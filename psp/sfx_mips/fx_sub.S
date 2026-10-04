/* Auto-generated MIPS Allegrex for fx_sub */
.set noreorder
.text
.globl mips_fx_sub
mips_fx_sub:
.set noreorder

fx_sub:
    # Load the register index into $t0
    move $t0, $a1

    # Load SREG and GSU.avReg[reg] into $t1 and $t2 respectively
    lw $t1, 104($a0)          # Load pvSreg into $t1
    lw $t2, 0($t1)            # Load SREG into $t2
    sll $t0, $t0, 2           # Multiply reg by 4 to get offset
    add $t0, $a0, $t0         # Add offset to GSU struct pointer
    lw $t3, 0($t0)            # Load GSU.avReg[reg] into $t3

    # Convert SREG and GSU.avReg[reg] to int16
    seb $t2, $t2              # Sign-extend SREG to int32
    seb $t3, $t3              # Sign-extend GSU.avReg[reg] to int32

    # Perform subtraction
    sub $t4, $t2, $t3         # s = SREG - GSU.avReg[reg]

    # Update vCarry
    slt $t5, $t4, $zero       # s < 0
    xori $t5, $t5, 1          # Invert to get s >= 0
    sw $t5, 124($a0)          # Store vCarry

    # Update vOverflow
    xor $t6, $t2, $t3         # SREG ^ GSU.avReg[reg]
    xor $t7, $t2, $t4         # SREG ^ s
    and $t6, $t6, $t7         # (SREG ^ GSU.avReg[reg]) & (SREG ^ s)
    andi $t6, $t6, 0x8000     # & 0x8000
    sw $t6, 128($a0)          # Store vOverflow

    # Update vSign
    sw $t4, 116($a0)          # Store vSign

    # Update vZero
    sw $t4, 120($a0)          # Store vZero

    # Update R15 (PC)
    lw $t8, 60($a0)           # Load R15 into $t8
    addiu $t8, $t8, 1         # R15++
    sw $t8, 60($a0)           # Store R15

    # Update DREG
    lw $t9, 100($a0)          # Load pvDreg into $t9
    andi $t4, $t4, 0xffff     # s & 0xffff
    sw $t4, 0($t9)            # Store DREG

    # Return
    jr $ra
    nop

.set reorder
