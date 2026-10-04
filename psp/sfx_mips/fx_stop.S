/* Auto-generated MIPS Allegrex for fx_stop */
.set noreorder
.text
.globl mips_fx_stop
mips_fx_stop:
.set noreorder

fx_stop:
    # Load the pointer to the GSU struct into $t0
    lw $t0, 0($a0)

    # Clear vCounter (offset 132)
    sw $zero, 132($t0)

    # Clear vPlotOptionReg (offset 136)
    sw $zero, 136($t0)

    # Set vPipe to 1 (offset 140)
    li $t1, 1
    sw $t1, 140($t0)

    # Clear flags (vSign, vZero, vCarry, vOverflow)
    sw $zero, 116($t0)  # vSign
    sw $zero, 120($t0)  # vZero
    sw $zero, 124($t0)  # vCarry
    sw $zero, 128($t0)  # vOverflow

    # Increment R15 (PC) (offset 60)
    lw $t2, 60($t0)
    addiu $t2, $t2, 1
    sw $t2, 60($t0)

    # Return
    jr $ra
    nop

    .set reorder
