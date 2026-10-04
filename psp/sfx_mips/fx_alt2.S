/* Auto-generated MIPS Allegrex for fx_alt2 */
.set noreorder
.text
.globl mips_fx_alt2
mips_fx_alt2:
.set noreorder

fx_alt2:
    # Load the pointer to the GSU struct from $a0
    lw $t0, 100($a0)  # Load pvDreg into $t0
    lw $t1, 104($a0)  # Load pvSreg into $t1

    # Set SF(ALT2) by writing to pvDreg
    sw $t1, 0($t0)    # Store pvSreg into pvDreg

    # Set CF(B) by writing to pvSreg
    sw $t0, 0($t1)    # Store pvDreg into pvSreg

    # Increment R15 (PC) by 1
    lw $t2, 60($a0)   # Load R15 (PC) into $t2
    addiu $t2, $t2, 1 # Increment R15 by 1
    sw $t2, 60($a0)   # Store the incremented R15 back

    # Return
    jr $ra
    nop               # Fill branch delay slot with nop

.set reorder
