/* Auto-generated MIPS Allegrex for fx_not */
.set noreorder
.text
.globl mips_fx_not
mips_fx_not:
.set noreorder

fx_not:
    # Load the pointer to the GSU struct from $a0
    lw $t0, 100($a0)          # Load pvDreg into $t0
    lw $t1, 104($a0)          # Load pvSreg into $t1
    lw $t2, 120($a0)          # Load vZero into $t2

    # Load the value from SREG (R14) and perform NOT operation
    lw $v0, 56($a0)           # Load R14 (SREG) into $v0
    not $v0, $v0              # Perform NOT operation on $v0
    andi $v0, $v0, 0xffff      # Mask the result to 0xffff

    # Store the result to DREG (R15)
    sw $v0, 60($a0)           # Store $v0 to R15 (DREG)

    # Increment R15 (PC)
    lw $t3, 60($a0)           # Load R15 (PC) into $t3
    addiu $t3, $t3, 1         # Increment R15 by 1
    sw $t3, 60($a0)           # Store the incremented value back to R15

    # Store the result to vSign
    sw $v0, 116($a0)          # Store $v0 to vSign

    # Store the result to vZero
    sw $v0, 120($a0)          # Store $v0 to vZero

    # Clear flags (vCarry, vOverflow)
    sw $zero, 124($a0)        # Clear vCarry
    sw $zero, 128($a0)        # Clear vOverflow

    # Return
    jr $ra
    nop                       # Fill branch delay slot with nop

    .set reorder
