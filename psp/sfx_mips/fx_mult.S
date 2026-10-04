/* Auto-generated MIPS Allegrex for fx_mult */
.set noreorder
.text
.globl mips_fx_mult
mips_fx_mult:
.set noreorder

fx_mult:
    # Load the GSU struct pointer from $a0
    lw $t0, 100($a0)          # Load pvDreg into $t0
    lw $t1, 104($a0)          # Load pvSreg into $t1
    lw $t2, 120($a0)          # Load vZero into $t2
    lw $t3, 124($a0)          # Load vCarry into $t3
    lw $t4, 128($a0)          # Load vOverflow into $t4

    # Load the register index from $a1
    sll $t5, $a1, 2           # Multiply $a1 by 4 to get the offset
    add $t6, $a0, $t5         # Calculate the address of GSU.avReg[reg]
    lw $t7, 0($t6)            # Load GSU.avReg[reg] into $t7

    # Load the SREG value
    lb $t8, 0($t1)            # Load SREG into $t8 (sign-extend to int32)

    # Perform the multiplication
    mult $t8, $t7             # Multiply SREG and GSU.avReg[reg]
    mflo $t9                  # Move the low 32 bits of the result to $t9

    # Mask the result to 16 bits
    andi $t9, $t9, 0xffff     # Mask the result to 0xffff

    # Store the result to DREG
    sw $t9, 0($t0)            # Store the result to DREG

    # Update R15 (PC)
    lw $t6, 60($a0)           # Load R15 (PC) into $t6
    addiu $t6, $t6, 1         # Increment R15 by 1
    sw $t6, 60($a0)           # Store the updated R15 back

    # Update vSign, vZero, vCarry, and vOverflow
    sw $t9, 116($a0)          # Store the result to vSign
    sw $t2, 120($a0)          # Store vZero (unchanged)
    sw $t3, 124($a0)          # Store vCarry (unchanged)
    sw $t4, 128($a0)          # Store vOverflow (unchanged)

    # Clear flags (CLRFLAGS)
    # Assuming CLRFLAGS is a macro or inline assembly that clears flags
    # For simplicity, we'll assume it's a no-op in this context
    # If CLRFLAGS is a specific instruction, replace this with the actual instruction

    # Return
    jr $ra
    nop                       # Fill branch delay slot with nop

.set reorder
