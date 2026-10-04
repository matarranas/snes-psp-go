/* Auto-generated MIPS Allegrex for fx_lob */
.set noreorder
.text
.globl mips_fx_lob
mips_fx_lob:
.set noreorder

fx_lob:
    # Load SREG pointer from GSU struct
    lw $t0, 104($a0)  # $t0 = pvSreg

    # Load SREG value
    lbu $v0, 0($t0)   # $v0 = SREG & 0xff

    # Increment R15 (PC)
    lw $t1, 60($a0)   # $t1 = R15
    addiu $t1, $t1, 1 # $t1 = R15 + 1
    sw $t1, 60($a0)   # R15 = $t1

    # Store DREG value
    lw $t2, 100($a0)  # $t2 = pvDreg
    sb $v0, 0($t2)    # DREG = $v0

    # Set vSign
    sll $t3, $v0, 8   # $t3 = v << 8
    sw $t3, 116($a0)  # vSign = $t3

    # Set vZero
    sw $t3, 120($a0)  # vZero = $t3

    # Clear flags
    sw $zero, 124($a0) # vCarry = 0
    sw $zero, 128($a0) # vOverflow = 0

    # Return
    jr $ra
    nop
