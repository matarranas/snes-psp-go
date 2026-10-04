/* Auto-generated MIPS Allegrex for fx_lsr */
.set noreorder
.text
.globl mips_fx_lsr
mips_fx_lsr:
.set noreorder

fx_lsr:
    # Load SREG pointer from GSU struct
    lw $t0, 104($a0)  # $t0 = pvSreg

    # Load SREG value
    lw $t1, 0($t0)    # $t1 = SREG

    # Save carry flag
    andi $t2, $t1, 1  # $t2 = SREG & 1
    sw $t2, 124($a0)  # GSU.vCarry = $t2

    # Shift SREG right by 1
    andi $t3, $t1, 0xffff  # $t3 = SREG & 0xffff
    srl $t3, $t3, 1       # $t3 = $t3 >> 1

    # Increment PC (R15)
    lw $t4, 60($a0)   # $t4 = R15
    addiu $t4, $t4, 1 # $t4 = R15 + 1
    sw $t4, 60($a0)   # R15 = $t4

    # Store result to DREG
    lw $t0, 100($a0)  # $t0 = pvDreg
    sw $t3, 0($t0)    # DREG = $t3

    # Update sign flag
    sw $t3, 116($a0)  # GSU.vSign = $t3

    # Update zero flag
    sw $t3, 120($a0)  # GSU.vZero = $t3

    # Clear flags
    sw $zero, 128($a0)  # GSU.vOverflow = 0

    # Return
    jr $ra
    nop
