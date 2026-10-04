/* Auto-generated MIPS Allegrex for fx_alt3 */
.set noreorder
.text
.globl mips_fx_alt3
mips_fx_alt3:
.set noreorder

fx_alt3:
    # Load the pointer to the GSU struct from $a0
    lw $t0, 100($a0)  # Load pvDreg into $t0
    lw $t1, 104($a0)  # Load pvSreg into $t1

    # SF(ALT1)
    lw $t2, 0($t0)    # Load D0 into $t2
    lw $t3, 0($t1)    # Load S0 into $t3
    addu $t4, $t2, $t3  # D0 + S0
    sw $t4, 0($t0)    # Store result back to D0

    # SF(ALT2)
    lw $t2, 4($t0)    # Load D1 into $t2
    lw $t3, 4($t1)    # Load S1 into $t3
    addu $t4, $t2, $t3  # D1 + S1
    sw $t4, 4($t0)    # Store result back to D1

    # CF(B)
    lw $t2, 124($a0)  # Load vCarry into $t2
    lw $t3, 128($a0)  # Load vOverflow into $t3
    or $t4, $t2, $t3  # vCarry | vOverflow
    sw $t4, 128($a0)  # Store result back to vOverflow

    # R15++
    lw $t2, 60($a0)   # Load R15 (PC) into $t2
    addiu $t2, $t2, 1 # Increment R15 by 1
    sw $t2, 60($a0)   # Store incremented R15 back to struct

    # Return
    jr $ra
    nop
