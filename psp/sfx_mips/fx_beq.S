/* Auto-generated MIPS Allegrex for fx_beq */
.set noreorder
.text
.globl mips_fx_beq
mips_fx_beq:
.set noreorder

fx_beq:
    # Load the GSU struct pointer from $a0
    lw $t0, 60($a0)          # Load R15 (PC) into $t0

    # Increment R15 (PC)
    addiu $t0, $t0, 1        # R15++

    # Store the incremented R15 back to the GSU struct
    sw $t0, 60($a0)          # Store R15 back to offset 60

    # Load vZero from the GSU struct
    lw $t1, 120($a0)         # Load vZero into $t1

    # Check if (vZero & 0xffff) == 0
    andi $t2, $t1, 0xffff    # $t2 = vZero & 0xffff
    beqz $t2, branch_taken  # If $t2 == 0, branch to branch_taken

    # If vZero & 0xffff != 0, increment R15 again
    addiu $t0, $t0, 1        # R15++
    sw $t0, 60($a0)          # Store R15 back to offset 60
    j end                   # Jump to end

branch_taken:
    # Load PIPE into $t3
    lbu $t3, 100($a0)        # Load PIPE into $t3

    # Sign-extend $t3 to 32 bits
    seb $t3, $t3            # Sign-extend $t3 to $t3

    # Add the sign-extended PIPE to R15
    add $t0, $t0, $t3        # R15 += (int32)(int8)v

    # Store the updated R15 back to the GSU struct
    sw $t0, 60($a0)          # Store R15 back to offset 60

end:
    # Return from the function
    jr $ra                  # Return to caller
    nop                     # Branch delay slot

    .set reorder
