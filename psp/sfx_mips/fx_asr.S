/* Auto-generated MIPS Allegrex for fx_asr */
.set noreorder
.text
.globl mips_fx_asr
mips_fx_asr:
.set noreorder

fx_asr:
    # Load SREG pointer from GSU struct
    lw $t0, 104($a0)  # $t0 = pvSreg

    # Load SREG value
    lw $t1, 0($t0)    # $t1 = SREG

    # Save carry flag
    andi $t2, $t1, 1  # $t2 = SREG & 1
    sw $t2, 124($a0)  # GSU.vCarry = $t2

    # Perform ASR operation
    sra $t3, $t1, 1   # $t3 = (int32)(int16)SREG >> 1

    # Mask result to 16 bits
    andi $t3, $t3, 0xffff  # $t3 = $t3 & 0xffff

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

    # Clear flags (vCarry, vOverflow)
    sw $zero, 124($a0)  # GSU.vCarry = 0
    sw $zero, 128($a0)  # GSU.vOverflow = 0

    # Return
    jr $ra
    nop

.set reorder
