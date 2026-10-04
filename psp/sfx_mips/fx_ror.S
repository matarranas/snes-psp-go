/* Auto-generated MIPS Allegrex for fx_ror */
.set noreorder
.text
.globl mips_fx_ror
mips_fx_ror:
.set noreorder

fx_ror:
    # Load SREG into $t0
    lw $t0, 104($a0)          # $t0 = GSU.pvSreg
    lw $t1, 0($t0)            # $t1 = *GSU.pvSreg (SREG)

    # Calculate the rotated value
    srl $t2, $t1, 1           # $t2 = SREG >> 1
    andi $t3, $t1, 1          # $t3 = SREG & 1
    sll $t3, $t3, 15          # $t3 = (SREG & 1) << 15
    or $t2, $t2, $t3          # $t2 = (SREG >> 1) | ((SREG & 1) << 15)

    # Update vCarry
    sw $t3, 124($a0)          # GSU.vCarry = (SREG & 1)

    # Increment R15 (PC)
    lw $t3, 60($a0)           # $t3 = R15
    addiu $t3, $t3, 1         # R15++
    sw $t3, 60($a0)           # Store back R15

    # Update DREG
    lw $t0, 100($a0)          # $t0 = GSU.pvDreg
    sw $t2, 0($t0)            # *GSU.pvDreg = $t2 (DREG = v)

    # Update vSign
    sw $t2, 116($a0)          # GSU.vSign = v

    # Update vZero
    beqz $t2, 1f              # if (v == 0)
    sw $zero, 120($a0)        # GSU.vZero = 0
    j 2f
1:
    sw $t2, 120($a0)          # GSU.vZero = v
2:

    # Clear flags (vOverflow is already cleared by the previous operation)
    sw $zero, 128($a0)        # GSU.vOverflow = 0

    # Return
    jr $ra
    nop

.set reorder
