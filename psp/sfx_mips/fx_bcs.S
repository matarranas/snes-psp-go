/* Auto-generated MIPS Allegrex for fx_bcs */
.set noreorder
.text
.globl mips_fx_bcs
mips_fx_bcs:
.set noreorder

fx_bcs:
    # Load the GSU struct pointer from $a0
    lw $t0, 60($a0)          # Load R15 (PC) into $t0

    # Increment R15 (PC)
    addiu $t0, $t0, 1        # R15++

    # Store the incremented R15 back to the GSU struct
    sw $t0, 60($a0)          # Store R15 back to offset 60

    # Load the vCarry flag
    lw $t1, 124($a0)         # Load vCarry into $t1

    # Check if vCarry is set
    andi $t2, $t1, 1         # $t2 = vCarry & 1

    # Load the PIPE value
    lbu $t3, 0($t0)          # Load the byte at the current PC into $t3

    # If vCarry is not set, increment R15 again
    beqz $t2, increment_pc   # If vCarry & 1 == 0, branch to increment_pc

    # If vCarry is set, add the signed byte value to R15
    add $t0, $t0, $t3        # R15 += (int32)(int8)v

increment_pc:
    # Store the final R15 value back to the GSU struct
    sw $t0, 60($a0)          # Store R15 back to offset 60

    # Return from the function
    jr $ra                  # Return to the caller
    nop                     # Branch delay slot

    .set reorder
