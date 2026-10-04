/* Auto-generated MIPS Allegrex for fx_bcc */
.set noreorder
.text
.globl mips_fx_bcc
mips_fx_bcc:
.set noreorder

fx_bcc:
    # Load the GSU struct pointer from $a0
    lw      $t0, 60($a0)          # Load R15 (PC) into $t0

    # Increment R15 (PC)
    addiu   $t0, $t0, 1          # R15++

    # Store the incremented R15 back to the GSU struct
    sw      $t0, 60($a0)          # Store R15 back to offset 60

    # Load the vCarry flag
    lw      $t1, 124($a0)         # Load vCarry into $t1

    # Check if vCarry is 0
    andi    $t2, $t1, 1          # $t2 = vCarry & 1
    beqz    $t2, fx_bcc_skip     # If vCarry is 0, skip the branch

    # If vCarry is 1, increment R15 again
    addiu   $t0, $t0, 1          # R15++
    sw      $t0, 60($a0)          # Store R15 back to offset 60
    j       fx_bcc_end           # Jump to the end

fx_bcc_skip:
    # Load the PIPE value (assuming PIPE is a byte at offset 0 in the GSU struct)
    lb      $t3, 0($a0)           # Load PIPE into $t3

    # Sign-extend the PIPE value to 32 bits
    sb      $t3, 128($a0)         # Store the sign-extended value to vOverflow (temporarily)
    lw      $t3, 128($a0)         # Load the sign-extended value back into $t3

    # Add the sign-extended PIPE value to R15
    addu    $t0, $t0, $t3        # R15 += (int32)(int8)v
    sw      $t0, 60($a0)          # Store R15 back to offset 60

fx_bcc_end:
    # Restore the return address and return
    jr      $ra                  # Return
    nop                         # Branch delay slot

    .set reorder
