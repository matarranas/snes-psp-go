/* Auto-generated MIPS Allegrex for fx_swap */
.set noreorder
.text
.globl mips_fx_swap
mips_fx_swap:
.set noreorder

fx_swap:
    # Load SREG into $t0
    lw $t0, 104($a0)  # $t0 = pvSreg
    lw $t1, 0($t0)    # $t1 = SREG

    # Extract low and high bytes of SREG
    andi $t2, $t1, 0xFF  # $t2 = c = SREG & 0xFF
    srl $t3, $t1, 8     # $t3 = d = SREG >> 8

    # Combine c and d into a 16-bit value
    or $t4, $t2, $t3    # $t4 = (c << 8) | d

    # Increment R15 (PC)
    lw $t5, 60($a0)    # $t5 = R15
    addiu $t5, $t5, 1   # $t5 = R15 + 1
    sw $t5, 60($a0)    # R15 = $t5

    # Store the combined value into DREG
    lw $t6, 100($a0)   # $t6 = pvDreg
    sw $t4, 0($t6)     # DREG = $t4

    # Set vSign, vZero, and clear flags
    sw $t4, 116($a0)   # vSign = $t4
    sw $t4, 120($a0)   # vZero = $t4
    sw $zero, 124($a0) # vCarry = 0
    sw $zero, 128($a0) # vOverflow = 0

    # Return
    jr $ra
    nop
