/* Auto-generated MIPS Allegrex for fx_bra */
.set noreorder
.text
.globl mips_fx_bra
mips_fx_bra:
.set noreorder

fx_bra:
    # Load the GSU struct pointer from $a0
    lw $t0, 60($a0)          # Load R15 (PC) into $t0

    # Increment R15 (PC)
    addiu $t0, $t0, 1         # R15++

    # Store the incremented R15 back to the GSU struct
    sw $t0, 60($a0)          # Store R15 back to offset 60

    # Load the PIPE value into $t1
    lbu $t1, 0($t0)          # Load PIPE value into $t1 (8-bit unsigned)

    # Sign-extend the PIPE value to 32-bit
    sb $t1, 0($sp)           # Store $t1 to stack to sign-extend
    lw $t1, 0($sp)           # Load sign-extended value back into $t1

    # Add the sign-extended PIPE value to R15
    add $t0, $t0, $t1         # R15 += (int32)(int8)v

    # Store the final R15 value back to the GSU struct
    sw $t0, 60($a0)          # Store R15 back to offset 60

    # Return from the function
    jr $ra                   # Return
    nop                      # Fill branch delay slot with nop

.set reorder
