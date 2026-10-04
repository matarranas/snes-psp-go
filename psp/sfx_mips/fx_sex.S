/* Auto-generated MIPS Allegrex for fx_sex */
.set noreorder
.text
.globl mips_fx_sex
mips_fx_sex:
.set noreorder

fx_sex:
    # Load SREG pointer from GSU struct
    lw $t0, 104($a0)  # $t0 = pvSreg

    # Load the sign-extended 8-bit value from SREG
    lb $t1, 0($t0)    # $t1 = (int8)SREG

    # Sign-extend the 8-bit value to 32-bit
    sb $t1, 0($t1)    # Sign-extend $t1 to 32-bit in $t1

    # Increment R15 (PC)
    lw $t2, 60($a0)   # $t2 = R15
    addiu $t2, $t2, 1 # R15++
    sw $t2, 60($a0)   # Store back R15

    # Store the result to DREG
    lw $t0, 100($a0)  # $t0 = pvDreg
    sw $t1, 0($t0)    # DREG = v

    # Update GSU.vSign
    sw $t1, 116($a0)  # GSU.vSign = v

    # Update GSU.vZero
    beqz $t1, 1f      # If v == 0, set GSU.vZero = 1
    sw $zero, 120($a0) # GSU.vZero = 0
    j 2f
1:  sw $zero, 120($a0) # GSU.vZero = 1

    # Clear flags
2:  sw $zero, 124($a0) # GSU.vCarry = 0
    sw $zero, 128($a0) # GSU.vOverflow = 0

    # Return
    jr $ra
    nop
