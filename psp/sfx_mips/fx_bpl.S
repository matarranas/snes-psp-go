/* Auto-generated MIPS Allegrex for fx_bpl */
.set noreorder
.text
.globl mips_fx_bpl
mips_fx_bpl:
.set noreorder

fx_bpl:
    # Load the GSU struct pointer from $a0
    lw $t0, 0($a0)          # Load R0 (PC) into $t0

    # Increment R15 (PC)
    addiu $t0, $t0, 1       # R15++ (PC++)

    # Store the incremented PC back to the GSU struct
    sw $t0, 0($a0)          # Store R0 (PC) back to offset 0

    # Load the PIPE value into $t1
    lbu $t1, 100($a0)       # Load PIPE value into $t1

    # Load the vSign value into $t2
    lw $t2, 116($a0)        # Load vSign into $t2

    # Check if vSign & 0x8000 is zero
    andi $t3, $t2, 0x8000   # $t3 = vSign & 0x8000
    beqz $t3, fx_bpl_skip   # If $t3 is zero, skip the branch

    # If vSign & 0x8000 is not zero, increment PC again
    addiu $t0, $t0, 1       # R15++ (PC++)

    # Store the incremented PC back to the GSU struct
    sw $t0, 0($a0)          # Store R0 (PC) back to offset 0

    # Jump to the end of the function
    j fx_bpl_end            # Jump to fx_bpl_end

fx_bpl_skip:
    # If vSign & 0x8000 is zero, add the PIPE value to PC
    addu $t0, $t0, $t1      # R15 += (int32)(int8)v

    # Store the incremented PC back to the GSU struct
    sw $t0, 0($a0)          # Store R0 (PC) back to offset 0

fx_bpl_end:
    # Return from the function
    jr $ra                  # Return to caller
    nop                     # Branch delay slot

    .set reorder
