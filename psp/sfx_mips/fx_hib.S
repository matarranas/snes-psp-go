/* Auto-generated MIPS Allegrex for fx_hib */
.set noreorder
.text
.globl mips_fx_hib
mips_fx_hib:
.set noreorder

fx_hib:
    # Load the pointer to the GSU struct into $t0
    lw $t0, 104($a0)  # Load pvSreg into $t0

    # Load the value from SREG (pvSreg) into $t1
    lw $t1, 0($t0)    # Load SREG into $t1

    # Extract the byte at position 8 and store it in $t2
    srl $t2, $t1, 8   # Shift right by 8 to get the byte at position 8
    andi $t2, $t2, 0xff  # Mask to get the lower 8 bits

    # Increment R15 (PC) by 1
    lw $t3, 60($a0)   # Load R15 (PC) into $t3
    addiu $t3, $t3, 1  # Increment R15 by 1
    sw $t3, 60($a0)   # Store the incremented R15 back

    # Store the extracted byte into DREG (pvDreg)
    lw $t0, 100($a0)  # Load pvDreg into $t0
    sw $t2, 0($t0)    # Store the byte into DREG

    # Set vSign to the byte shifted left by 8
    sll $t3, $t2, 8   # Shift left by 8
    sw $t3, 116($a0)  # Store into vSign

    # Set vZero to the byte shifted left by 8
    sw $t3, 120($a0)  # Store into vZero

    # Clear flags (vCarry, vOverflow)
    sw $zero, 124($a0) # Clear vCarry
    sw $zero, 128($a0) # Clear vOverflow

    # Return
    jr $ra
    nop

    .set reorder
