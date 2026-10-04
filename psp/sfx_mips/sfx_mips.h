#ifndef _SFX_MIPS_H_
#define _SFX_MIPS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "fxinst.h"

/* Pointer to GSU state */
extern struct FxRegs_s GSU;

/* Native MIPS Allegrex Super FX Opcode Implementations */
void mips_fx_stop(struct FxRegs_s *gsu);
void mips_fx_nop(struct FxRegs_s *gsu);
void mips_fx_cache(struct FxRegs_s *gsu);
void mips_fx_lsr(struct FxRegs_s *gsu);
void mips_fx_rol(struct FxRegs_s *gsu);
void mips_fx_bra(struct FxRegs_s *gsu);
void mips_fx_bne(struct FxRegs_s *gsu);
void mips_fx_beq(struct FxRegs_s *gsu);
void mips_fx_bpl(struct FxRegs_s *gsu);
void mips_fx_bmi(struct FxRegs_s *gsu);
void mips_fx_bcc(struct FxRegs_s *gsu);
void mips_fx_bcs(struct FxRegs_s *gsu);
void mips_fx_loop(struct FxRegs_s *gsu);
void mips_fx_alt1(struct FxRegs_s *gsu);
void mips_fx_alt2(struct FxRegs_s *gsu);
void mips_fx_alt3(struct FxRegs_s *gsu);
void mips_fx_swap(struct FxRegs_s *gsu);
void mips_fx_not(struct FxRegs_s *gsu);
void mips_fx_merge(struct FxRegs_s *gsu);
void mips_fx_sex(struct FxRegs_s *gsu);
void mips_fx_asr(struct FxRegs_s *gsu);
void mips_fx_ror(struct FxRegs_s *gsu);
void mips_fx_lob(struct FxRegs_s *gsu);
void mips_fx_hib(struct FxRegs_s *gsu);

/* Register parameterized functions */
void mips_fx_add(struct FxRegs_s *gsu, int reg);
void mips_fx_adc(struct FxRegs_s *gsu, int reg);
void mips_fx_sub(struct FxRegs_s *gsu, int reg);
void mips_fx_sbc(struct FxRegs_s *gsu, int reg);
void mips_fx_cmp(struct FxRegs_s *gsu, int reg);
void mips_fx_and(struct FxRegs_s *gsu, int reg);
void mips_fx_bic(struct FxRegs_s *gsu, int reg);
void mips_fx_or(struct FxRegs_s *gsu, int reg);
void mips_fx_xor(struct FxRegs_s *gsu, int reg);
void mips_fx_inc(struct FxRegs_s *gsu, int reg);
void mips_fx_dec(struct FxRegs_s *gsu, int reg);
void mips_fx_mult(struct FxRegs_s *gsu, int reg);
void mips_fx_umult(struct FxRegs_s *gsu, int reg);
void mips_fx_fmult(struct FxRegs_s *gsu);

#ifdef __cplusplus
}
#endif

#endif /* _SFX_MIPS_H_ */
