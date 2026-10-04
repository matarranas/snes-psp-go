/* Auto-generated MIPS Allegrex for fx_loop */
.set noreorder
.text
.globl mips_fx_loop
mips_fx_loop:
.set noreorder

fx_loop:
    lw      $t0, 60($a0)          # Load R12 into $t0
    addiu   $t0, $t0, -1          # Decrement R12
    sw      $t0, 60($a0)          # Store decremented R12 back to R12

    sw      $t0, 116($a0)         # Set vSign = R12
    sw      $t0, 120($a0)         # Set vZero = R12

    andi    $t1, $t0, 0xFFFF      # Extract lower 16 bits of R12
    beqz    $t1, fx_loop_else    # If lower 16 bits are zero, branch to fx_loop_else

    lw      $t2, 52($a0)          # Load R13 into $t2
    sw      $t2, 60($a0)          # Set R15 = R13
    j       fx_loop_end           # Jump to fx_loop_end

fx_loop_else:
    lw      $t2, 60($a0)          # Load R15 into $t2
    addiu   $t2, $t2, 1          # Increment R15
    sw      $t2, 60($a0)          # Store incremented R15 back to R15

fx_loop_end:
    sw      $zero, 124($a0)       # Clear vCarry
    sw      $zero, 128($a0)       # Clear vOverflow

    jr      $ra                  # Return
    nop                         # Branch delay slot

    .set reorder
