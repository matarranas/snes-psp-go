/* Auto-generated MIPS Allegrex for fx_umult */
.set noreorder
.text
.globl mips_fx_umult
mips_fx_umult:
.set noreorder

fx_umult:
    # Load the GSU struct pointer from $a0
    lw $t0, 104($a0)          # Load pvSreg into $t0
    lw $t1, 0($t0)            # Load SREG into $t1

    # Load the register index from $a1
    lw $t2, 0($a0)            # Load R0 into $t2
    sll $t3, $a1, 2           # Multiply register index by 4
    add $t2, $t2, $t3         # Add offset to R0 to get the desired register
    lw $t4, 0($t2)            # Load GSU.avReg[reg] into $t4

    # Perform the multiplication and mask the result
    andi $t1, $t1, 0xff        # Mask SREG to 0xff
    andi $t4, $t4, 0xff        # Mask GSU.avReg[reg] to 0xff
    multu $t1, $t4            # Unsigned multiply
    mflo $t5                  # Move result to $t5
    andi $t5, $t5, 0xffff      # Mask result to 0xffff

    # Increment R15 (PC)
    lw $t6, 60($a0)           # Load R15 into $t6
    addiu $t6, $t6, 1         # Increment R15 by 1
    sw $t6, 60($a0)           # Store R15 back

    # Store the result to DREG
    lw $t7, 100($a0)          # Load pvDreg into $t7
    sw $t5, 0($t7)            # Store result to DREG

    # Update GSU.vSign, GSU.vZero
    sw $t5, 116($a0)          # Store result to GSU.vSign
    beqz $t5, 1f              # If result is zero, skip setting vZero
    sw $zero, 120($a0)        # Store 0 to GSU.vZero
    j 2f                      # Jump to end
1:  sw $t5, 120($a0)          # Store result to GSU.vZero
2:  # Clear flags
    sw $zero, 124($a0)        # Clear vCarry
    sw $zero, 128($a0)        # Clear vOverflow

    # Return
    jr $ra
    nop                       # Branch delay slot

    .set reorder
