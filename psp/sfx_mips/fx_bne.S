/* Auto-generated MIPS Allegrex for fx_bne */
.set noreorder
.text
.globl mips_fx_bne
mips_fx_bne:
.set noreorder

fx_bne:
    # Load the GSU struct pointer from $a0
    lw $t0, 0($a0)          # Load R0 (PC) into $t0

    # Increment R15 (PC)
    addiu $t0, $t0, 1       # R15++ (PC++)

    # Store the incremented PC back to the GSU struct
    sw $t0, 0($a0)          # Store R0 (PC) back

    # Load the vZero value from the GSU struct
    lw $t1, 120($a0)        # Load vZero into $t1

    # Check if (vZero & 0xffff) != 0
    andi $t2, $t1, 0xffff   # $t2 = vZero & 0xffff
    beqz $t2, increment_pc  # If $t2 == 0, branch to increment_pc

    # Load the PIPE value from the GSU struct
    lbu $t3, 100($a0)       # Load PIPE into $t3 (8-bit unsigned)

    # Sign-extend the PIPE value to 32-bit
    sb $t3, 100($a0)       # Store the sign-extended value back to pvDreg
    lb $t3, 100($a0)       # Load the sign-extended value into $t3

    # Add the sign-extended PIPE value to R15 (PC)
    add $t0, $t0, $t3       # R15 += (int32)(int8)v

    # Store the updated PC back to the GSU struct
    sw $t0, 0($a0)          # Store R0 (PC) back

    # Jump to the end of the function
    j end_fx_bne
    nop                     # Fill branch delay slot

increment_pc:
    # Increment R15 (PC) again
    addiu $t0, $t0, 1       # R15++ (PC++)

    # Store the incremented PC back to the GSU struct
    sw $t0, 0($a0)          # Store R0 (PC) back

end_fx_bne:
    # Return from the function
    jr $ra
    nop                     # Fill branch delay slot

    .set reorder
