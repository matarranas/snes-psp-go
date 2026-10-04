/* Auto-generated MIPS Allegrex for fx_nop */
.set noreorder
.text
.globl mips_fx_nop
mips_fx_nop:
.set noreorder

fx_nop:
    lw      $t0, 128($a0)          # Load vOverflow into $t0
    sw      $zero, 128($a0)        # Clear vOverflow
    sw      $zero, 124($a0)        # Clear vCarry
    sw      $zero, 120($a0)        # Clear vZero
    sw      $zero, 116($a0)        # Clear vSign

    lw      $t1, 60($a0)           # Load R15 (PC) into $t1
    addiu   $t1, $t1, 1           # Increment R15 (PC) by 1
    sw      $t1, 60($a0)           # Store incremented R15 back to struct

    jr      $ra                   # Return
    nop                           # Fill branch delay slot with nop

    .set reorder
