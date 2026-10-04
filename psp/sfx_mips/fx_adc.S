/* Auto-generated MIPS Allegrex for fx_adc */
.set noreorder
.text
.globl mips_fx_adc
mips_fx_adc:
.set noreorder

fx_adc:
    # Load the register index into $t0
    move $t0, $a1

    # Load SREG into $t1
    lw $t1, 104($a0)

    # Load the value of GSU.avReg[reg] into $t2
    sll $t0, $t0, 2          # Multiply reg by 4
    addu $t0, $a0, $t0       # Add base address of GSU struct
    lw $t2, 0($t0)           # Load the value

    # Load vCarry into $t3
    lw $t3, 124($a0)

    # Calculate s = (int32)(int16)SREG + (int32)(int16)GSU.avReg[reg] + (GSU.vCarry & 1)
    addu $t4, $t1, $t2       # s = SREG + GSU.avReg[reg]
    addu $t4, $t4, $t3       # s += vCarry

    # Update vCarry: GSU.vCarry = s >= 0x10000
    sltu $t3, $t4, 0x10000   # Set $t3 to 1 if s < 0x10000, else 0
    sw $t3, 124($a0)         # Store vCarry

    # Update vOverflow: GSU.vOverflow = ~(SREG ^ GSU.avReg[reg]) & (GSU.avReg[reg] ^ s) & 0x8000
    xor $t5, $t1, $t2       # SREG ^ GSU.avReg[reg]
    xor $t6, $t2, $t4       # GSU.avReg[reg] ^ s
    not $t5, $t5            # ~(SREG ^ GSU.avReg[reg])
    and $t5, $t5, $t6       # ~(SREG ^ GSU.avReg[reg]) & (GSU.avReg[reg] ^ s)
    andi $t5, $t5, 0x8000    # & 0x8000
    sw $t5, 128($a0)         # Store vOverflow

    # Update vSign: GSU.vSign = s
    sw $t4, 116($a0)         # Store vSign

    # Update vZero: GSU.vZero = s
    sw $t4, 120($a0)         # Store vZero

    # Increment R15: R15++
    lw $t0, 60($a0)          # Load R15
    addiu $t0, $t0, 1        # Increment R15
    sw $t0, 60($a0)          # Store R15

    # Update DREG: DREG = s & 0xffff
    lw $t0, 100($a0)         # Load pvDreg
    andi $t4, $t4, 0xffff    # s & 0xffff
    sw $t4, 0($t0)           # Store DREG

    # Clear flags (CLRFLAGS)
    # Assuming CLRFLAGS is a macro or function call that clears flags
    # If CLRFLAGS is a simple instruction, replace this with the actual instruction
    # For example, if CLRFLAGS is a no-op, you can use:
    # nop

    # Return
    jr $ra
    nop
