/* Auto-generated MIPS Allegrex for fx_merge */
.set noreorder
.text
.globl mips_fx_merge
mips_fx_merge:
.set noreorder

fx_merge:
    # Load R7 and R8 from the GSU struct
    lw $t0, 28($a0)  # R7
    lw $t1, 32($a0)  # R8

    # Merge R7 and R8 into $v0
    andi $v0, $t0, 0xff00
    andi $t1, $t1, 0xff00
    srl $t1, $t1, 8
    or $v0, $v0, $t1

    # Increment R15 (PC)
    lw $t2, 60($a0)  # R15
    addiu $t2, $t2, 1
    sw $t2, 60($a0)  # R15

    # Store the merged value to DREG
    lw $t3, 100($a0)  # pvDreg
    sw $v0, 0($t3)    # DREG = v

    # Update vZero
    andi $t4, $v0, 0xf0f0
    beqz $t4, fx_merge_vZero_set
    sw $zero, 120($a0)  # GSU.vZero = 0
    j fx_merge_vSign
fx_merge_vZero_set:
    li $t4, 1
    sw $t4, 120($a0)  # GSU.vZero = 1

fx_merge_vSign:
    # Update vSign
    or $t5, $v0, $t0
    andi $t5, $t5, 0x8000
    sw $t5, 116($a0)  # GSU.vSign = vSign

    # Update vCarry
    andi $t6, $v0, 0xe0e0
    bnez $t6, fx_merge_vCarry_set
    sw $zero, 124($a0)  # GSU.vCarry = 0
    j fx_merge_end
fx_merge_vCarry_set:
    li $t6, 1
    sw $t6, 124($a0)  # GSU.vCarry = 1

fx_merge_end:
    # Clear flags (CLRFLAGS)
    sw $zero, 128($a0)  # GSU.vOverflow = 0

    # Return
    jr $ra
    nop
