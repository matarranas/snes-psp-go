/* Auto-generated MIPS Allegrex for fx_cache */
.set noreorder
.text
.globl mips_fx_cache
mips_fx_cache:
.set noreorder

fx_cache:
    # Load R15 value from GSU struct
    lw      $t0, 60($a0)          # $t0 = R15

    # Calculate cache base address
    andi    $t1, $t0, 0xfff0      # $t1 = R15 & 0xfff0

    # Store cache base address to GSU struct
    sw      $t1, 132($a0)         # GSU.vCacheBaseReg = $t1

    # Set cache active flag
    li      $t2, 1                # $t2 = TRUE
    sw      $t2, 136($a0)         # GSU.bCacheActive = TRUE

    # Increment R15
    addiu   $t0, $t0, 1           # R15++
    sw      $t0, 60($a0)          # Store back R15

    # Clear flags
    sw      $zero, 116($a0)       # GSU.vSign = 0
    sw      $zero, 120($a0)       # GSU.vZero = 0
    sw      $zero, 124($a0)       # GSU.vCarry = 0
    sw      $zero, 128($a0)       # GSU.vOverflow = 0

    # Return
    jr      $ra
    nop

    .set reorder
