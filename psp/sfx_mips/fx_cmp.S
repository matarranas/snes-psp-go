/* Auto-generated MIPS Allegrex for fx_cmp */
.set noreorder
.text
.globl mips_fx_cmp
mips_fx_cmp:
.set noreorder

fx_cmp:
    # Load the register index into $t0
    move $t0, $a1

    # Load SREG into $t1
    lw $t1, 104($a0)

    # Load the register value into $t2
    sll $t0, $t0, 2
    addu $t0, $a0, $t0
    lw $t2, 0($t0)

    # Sign-extend SREG and the register value to 32 bits
    seb $t1, $t1
    seb $t2, $t2

    # Subtract the register value from SREG
    sub $t3, $t1, $t2

    # Update vCarry
    slt $t4, $t3, $zero
    sw $t4, 124($a0)

    # Update vOverflow
    xor $t5, $t1, $t2
    xor $t6, $t1, $t3
    and $t5, $t5, $t6
    andi $t5, $t5, 0x8000
    sw $t5, 128($a0)

    # Update vSign
    sw $t3, 116($a0)

    # Update vZero
    sw $t3, 120($a0)

    # Increment R15 (PC)
    lw $t7, 60($a0)
    addiu $t7, $t7, 1
    sw $t7, 60($a0)

    # Clear flags (CLRFLAGS)
    # Assuming CLRFLAGS is a macro or inline assembly that clears flags
    # For the sake of this example, we'll assume it's a no-op
    nop

    # Return
    jr $ra
    nop

.set reorder
