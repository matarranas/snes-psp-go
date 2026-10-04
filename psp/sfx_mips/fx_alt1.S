/* Auto-generated MIPS Allegrex for fx_alt1 */
.set noreorder
.text
.globl mips_fx_alt1
mips_fx_alt1:
.set noreorder

fx_alt1:
    # Load the pointer to the GSU struct from $a0
    lw $t0, 100($a0)  # Load pvDreg into $t0
    lw $t1, 104($a0)  # Load pvSreg into $t1

    # Set SF(ALT1) by writing to pvDreg
    sw $t0, 0($t0)    # Store pvDreg value to pvDreg (SF(ALT1))

    # Set CF(B) by writing to pvSreg
    sw $t1, 0($t1)    # Store pvSreg value to pvSreg (CF(B))

    # Increment R15 (PC) by 1
    lw $t2, 60($a0)   # Load R15 (PC) into $t2
    addiu $t2, $t2, 1 # Increment R15 by 1
    sw $t2, 60($a0)   # Store incremented R15 back to its location

    # Return
    jr $ra
    nop               # Fill branch delay slot with nop

.set reorder
