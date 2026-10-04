/* Auto-generated MIPS Allegrex for fx_bic */
.set noreorder
.text
.globl mips_fx_bic
mips_fx_bic:
.set noreorder

fx_bic:
    # Load the GSU struct pointer from $a0
    lw      $t0, 104($a0)          # Load pvSreg into $t0
    lw      $t1, 100($a0)          # Load pvDreg into $t1
    lw      $t2, 0($a0)            # Load R0 into $t2 (PC)
    lw      $t3, 4($a0)            # Load R1 into $t3
    lw      $t4, 8($a0)            # Load R2 into $t4
    lw      $t5, 12($a0)           # Load R3 into $t5
    lw      $t6, 16($a0)           # Load R4 into $t6
    lw      $t7, 20($a0)           # Load R5 into $t7
    lw      $t8, 24($a0)           # Load R6 into $t8
    lw      $t9, 28($a0)           # Load R7 into $t9
    lw      $s0, 32($a0)           # Load R8 into $s0
    lw      $s1, 36($a0)           # Load R9 into $s1
    lw      $s2, 40($a0)           # Load R10 into $s2
    lw      $s3, 44($a0)           # Load R11 into $s3
    lw      $s4, 48($a0)           # Load R12 into $s4
    lw      $s5, 52($a0)           # Load R13 into $s5
    lw      $s6, 56($a0)           # Load R14 into $s6
    lw      $s7, 60($a0)           # Load R15 into $s7 (PC)

    # Load the register value from avReg[reg]
    sll     $t8, $a1, 2            # Multiply reg by 4 to get offset
    addu    $t9, $a0, $t8          # Calculate address of avReg[reg]
    lw      $t8, 0($t9)            # Load avReg[reg] into $t8

    # Perform the bitwise AND NOT operation
    not     $t8, $t8                # Invert avReg[reg]
    and     $t8, $t0, $t8          # SREG & ~avReg[reg]

    # Store the result back to DREG and R15
    sw      $t8, 0($t1)            # Store result to DREG
    addiu   $s7, $s7, 1            # Increment R15 (PC)
    sw      $s7, 60($a0)           # Store updated R15 back to GSU struct

    # Update vSign, vZero, and clear flags
    sw      $t8, 116($a0)          # Update vSign
    sw      $t8, 120($a0)          # Update vZero
    sw      $zero, 124($a0)        # Clear vCarry
    sw      $zero, 128($a0)        # Clear vOverflow

    # Restore registers and return
    sw      $t2, 0($a0)            # Restore R0
    sw      $t3, 4($a0)            # Restore R1
    sw      $t4, 8($a0)            # Restore R2
    sw      $t5, 12($a0)           # Restore R3
    sw      $t6, 16($a0)           # Restore R4
    sw      $t7, 20($a0)           # Restore R5
    sw      $t8, 24($a0)           # Restore R6
    sw      $t9, 28($a0)           # Restore R7
    sw      $s0, 32($a0)           # Restore R8
    sw      $s1, 36($a0)           # Restore R9
    sw      $s2, 40($a0)           # Restore R10
    sw      $s3, 44($a0)           # Restore R11
    sw      $s4, 48($a0)           # Restore R12
    sw      $s5, 52($a0)           # Restore R13
    sw      $s6, 56($a0)           # Restore R14

    jr      $ra                    # Return
    nop                             # Branch delay slot
