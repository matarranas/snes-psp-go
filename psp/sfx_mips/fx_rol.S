/* Auto-generated MIPS Allegrex for fx_rol */
.set noreorder
.text
.globl mips_fx_rol
mips_fx_rol:
.set noreorder

fx_rol:
    lw      $t0, 104($a0)          # Load pvSreg into $t0
    lw      $t1, 124($a0)          # Load vCarry into $t1
    lw      $t2, 0($t0)            # Load SREG into $t2
    sll     $t3, $t2, 1            # Shift SREG left by 1
    addu    $t3, $t3, $t1          # Add vCarry to shifted SREG
    andi    $t3, $t3, 0xffff       # Mask lower 16 bits

    sw      $t3, 100($a0)          # Store result to DREG
    sw      $t3, 60($a0)           # Increment R15 (PC)
    sw      $t3, 116($a0)          # Store result to vSign
    sw      $t3, 120($a0)          # Store result to vZero

    srl     $t1, $t2, 15           # Shift SREG right by 15
    andi    $t1, $t1, 1            # Mask lower bit to get carry
    sw      $t1, 124($a0)          # Store new vCarry

    sw      $zero, 128($a0)        # Clear vOverflow

    jr      $ra
    nop
