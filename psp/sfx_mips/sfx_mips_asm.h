/*
 * sfx_mips_asm.h - Offsets and constants for Super FX MIPS Allegrex Core
 */

#ifndef _SFX_MIPS_ASM_H_
#define _SFX_MIPS_ASM_H_

/* Offsets in struct FxRegs_s GSU */
#define GSU_R0_OFF        0
#define GSU_R1_OFF        4
#define GSU_R2_OFF        8
#define GSU_R3_OFF        12
#define GSU_R4_OFF        16
#define GSU_R5_OFF        20
#define GSU_R6_OFF        24
#define GSU_R7_OFF        28
#define GSU_R8_OFF        32
#define GSU_R9_OFF        36
#define GSU_R10_OFF       40
#define GSU_R11_OFF       44
#define GSU_R12_OFF       48
#define GSU_R13_OFF       52
#define GSU_R14_OFF       56
#define GSU_R15_OFF       60

#define GSU_COLOR_OFF     64
#define GSU_PLOTOPT_OFF   68
#define GSU_STATUS_OFF    72
#define GSU_PRGBANK_OFF   76
#define GSU_ROMBANK_OFF   80
#define GSU_RAMBANK_OFF   84
#define GSU_CACHEBASE_OFF 88
#define GSU_CACHEFLAG_OFF 92
#define GSU_LASTRAM_OFF   96
#define GSU_DREG_OFF      100
#define GSU_SREG_OFF      104

#define GSU_VSIGN_OFF     116
#define GSU_VZERO_OFF     120
#define GSU_VCARRY_OFF    124
#define GSU_VOVERFLOW_OFF 128

#define GSU_PBR_PTR       328
#define GSU_VCOUNT_OFF    852

#endif /* _SFX_MIPS_ASM_H_ */
