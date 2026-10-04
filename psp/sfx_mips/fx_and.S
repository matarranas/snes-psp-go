/* Auto-generated MIPS Allegrex for fx_and */
.set noreorder
.text
.globl mips_fx_and
mips_fx_and:
.set noreorder

fx_and:
    # Load the pointer to the GSU struct from $a0
    lw $t0, 104($a0)  # Load pvSreg into $t0
    lw $t1, 0($a0)    # Load R0 into $t1 (PC)
    lw $t2, 100($a0)  # Load pvDreg into $t2

    # Calculate the address of the register to AND with SREG
    sll $t3, $a1, 2   # Multiply reg by 4 to get the offset
    add $t3, $t3, $t0 # Add offset to pvSreg to get the address of SREG

    # Load the value of SREG
    lw $t4, 0($t3)    # Load SREG into $t4

    # Calculate the address of the register to store the result
    sll $t5, $a1, 2   # Multiply reg by 4 to get the offset
    add $t5, $t5, $t2 # Add offset to pvDreg to get the address of DREG

    # Perform the AND operation
    and $t6, $t4, $t1 # AND SREG with R0 (PC) and store in $t6

    # Store the result in DREG
    sw $t6, 0($t5)    # Store $t6 into DREG

    # Update R15 (PC)
    addiu $t1, $t1, 1 # Increment R15 (PC) by 1
    sw $t1, 0($a0)    # Store the updated R15 back into the struct

    # Update vSign
    sw $t6, 116($a0) # Store the result into vSign

    # Update vZero
    beqz $t6, 1f     # If the result is zero, set vZero to 1
    sw $zero, 120($a0) # Otherwise, set vZero to 0
1:
    # Update vCarry
    sw $zero, 124($a0) # Clear vCarry

    # Update vOverflow
    sw $zero, 128($a0) # Clear vOverflow

    # Return
    jr $ra
    nop
