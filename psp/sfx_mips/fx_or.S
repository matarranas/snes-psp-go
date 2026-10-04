/* Auto-generated MIPS Allegrex for fx_or */
.set noreorder
.text
.globl mips_fx_or
mips_fx_or:
.set noreorder

fx_or:
    # Load the pointer to the GSU struct into $t0
    lw      $t0, 0($a0)

    # Load the register index into $t1
    move    $t1, $a1

    # Calculate the offset for the register array
    sll     $t1, $t1, 2          # Multiply reg by 4

    # Load the value from GSU.avReg[reg] into $t2
    lw      $t2, 60($t0)         # Load R15 (PC) into $t2
    addu    $t2, $t2, $t1        # Add the offset to get the correct register address
    lw      $t3, 0($t2)          # Load the value from GSU.avReg[reg]

    # Load the value from pvSreg into $t4
    lw      $t4, 104($t0)        # Load pvSreg pointer
    lw      $t4, 0($t4)          # Load the value from pvSreg

    # Perform the OR operation and mask with 0xffff
    or      $t5, $t3, $t4
    andi    $t5, $t5, 0xffff

    # Increment R15 (PC)
    addiu   $t2, $t2, 4          # Move to the next register (R15)
    lw      $t6, 0($t2)          # Load R15 (PC)
    addiu   $t6, $t6, 1          # Increment R15
    sw      $t6, 0($t2)          # Store the incremented R15 back

    # Store the result into pvDreg
    lw      $t7, 100($t0)        # Load pvDreg pointer
    sw      $t5, 0($t7)          # Store the result into pvDreg

    # Update vSign
    sw      $t5, 116($t0)        # Store the result into vSign

    # Update vZero
    beqz    $t5, 1f             # If result is zero, set vZero to 1
    sw      $zero, 120($t0)      # Store 0 into vZero
    j       2f
1:
    li      $t8, 1
    sw      $t8, 120($t0)        # Store 1 into vZero
2:

    # Clear flags (vCarry, vOverflow)
    sw      $zero, 124($t0)      # Clear vCarry
    sw      $zero, 128($t0)      # Clear vOverflow

    # Return
    jr      $ra
    nop
