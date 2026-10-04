/* Phase 3B: ROM Fetch Handlers (GETC, GETB, GETBL, GETBH, GETBS) */

.Lop_getc:
    lw $t0, GSU_ROMBUF_OFF($s0)  # Load byte from GSU.vRomBuffer
    lb $t1, GSU_PLOTOPT_OFF($s0)  # Load vPlotOptionReg
    and $t2, $t1, 0x04           # Check if vPlotOptionReg & 0x04
    beqz $t2, .Lop_getc_noshift   # If not, skip shifting
    srl $t0, $t0, 4               # Shift right by 4
    andi $t0, $t0, 0xF0           # Mask lower 4 bits
.Lop_getc_noshift:
    and $t1, $t1, 0x08           # Check if vPlotOptionReg & 0x08
    beqz $t1, .Lop_getc_setcolor  # If not, skip color modification
    lb $t3, GSU_COLOR_OFF($s0)    # Load vColorReg
    andi $t3, $t3, 0xF0           # Mask lower 4 bits
    or $t3, $t3, $t0              # Combine with shifted byte
    sb $t3, GSU_COLOR_OFF($s0)    # Store back to vColorReg
    j .Lfx_dispatch_loop         # Jump to dispatch loop
.Lop_getc_setcolor:
    sb $t0, GSU_COLOR_OFF($s0)    # Store byte to vColorReg
    j .Lfx_dispatch_loop         # Jump to dispatch loop

.Lop_getb:
    lb $t0, GSU_ROMBUF_OFF($s0)  # Load byte from GSU.vRomBuffer
    sw $t0, 0($s5)                # Store in DREG
    j .Lfx_dispatch_loop         # Jump to dispatch loop

.Lop_getbl:
    lb $t0, GSU_ROMBUF_OFF($s0)  # Load byte from GSU.vRomBuffer
    lw $t1, 0($s5)                # Load current DREG value
    andi $t1, $t1, 0xFF00         # Mask high byte
    or $t1, $t1, $t0              # Combine with new byte
    sw $t1, 0($s5)                # Store back to DREG
    j .Lfx_dispatch_loop         # Jump to dispatch loop

.Lop_getbh:
    lb $t0, GSU_ROMBUF_OFF($s0)  # Load byte from GSU.vRomBuffer
    sll $t0, $t0, 8               # Shift left by 8
    lw $t1, 0($s5)                # Load current DREG value
    andi $t1, $t1, 0x00FF         # Mask low byte
    or $t1, $t1, $t0              # Combine with new byte
    sw $t1, 0($s5)                # Store back to DREG
    j .Lfx_dispatch_loop         # Jump to dispatch loop

.Lop_getbs:
    lb $t0, GSU_ROMBUF_OFF($s0)  # Load byte from GSU.vRomBuffer
    seb $t0, $t0                 # Sign-extend byte
    sw $t0, 0($s5)                # Store in DREG
    j .Lfx_dispatch_loop         # Jump to dispatch loop
