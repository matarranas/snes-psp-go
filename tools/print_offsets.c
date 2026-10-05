#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define FX_RAM_BANKS 4

struct FxRegs_s
{
    uint32_t	avReg[16];
    uint32_t	vColorReg;
    uint32_t	vPlotOptionReg;
    uint32_t	vStatusReg;
    uint32_t	vPrgBankReg;
    uint32_t	vRomBankReg;
    uint32_t	vRamBankReg;
    uint32_t	vCacheBaseReg;
    uint32_t	vCacheFlags;
    uint32_t	vLastRamAdr;
    uint32_t *	pvDreg;
    uint32_t *	pvSreg;
    uint8_t	vRomBuffer;
    uint8_t	vPipe;
    uint32_t	vPipeAdr;

    uint32_t	vSign;
    uint32_t	vZero;
    uint32_t	vCarry;
    int32_t	vOverflow;
    
    int32_t	vErrorCode;
    uint32_t	vIllegalAddress;
    
    uint8_t	bBreakPoint;
    uint32_t	vBreakPoint;
    uint32_t	vStepPoint;
    
    uint8_t *	pvRegisters;
    uint32_t	nRamBanks;
    uint8_t *	pvRam;
    uint32_t	nRomBanks;
    uint8_t *   pvRom;

    uint32_t	vMode;
    uint32_t	vPrevMode;
    uint8_t *	pvScreenBase;
    uint8_t *	apvScreen[32];
    int		x[32];
    uint32_t	vScreenHeight;
    uint32_t	vScreenRealHeight;
    uint32_t	vPrevScreenHeight;
    uint32_t	vScreenSize;
    void	(*pfPlot)();
    void	(*pfRpix)();
    
    uint8_t *	pvRamBank;
    uint8_t *	pvRomBank;
    uint8_t *	pvPrgBank;

    uint8_t *	apvRamBank[FX_RAM_BANKS];
    uint8_t *	apvRomBank[256];

    uint8_t	bCacheActive;
    uint8_t *	pvCache;
    uint8_t 	avCacheBackup[512];
    uint32_t	vCounter;
    uint32_t	vInstCount;
    uint32_t	vSCBRDirty;
};

int main() {
    printf("GSU_COLOR_OFF     %zu\n", offsetof(struct FxRegs_s, vColorReg));
    printf("GSU_PLOTOPT_OFF   %zu\n", offsetof(struct FxRegs_s, vPlotOptionReg));
    printf("GSU_STATUS_OFF    %zu\n", offsetof(struct FxRegs_s, vStatusReg));
    printf("GSU_PRGBANK_OFF   %zu\n", offsetof(struct FxRegs_s, vPrgBankReg));
    printf("GSU_ROMBANK_OFF   %zu\n", offsetof(struct FxRegs_s, vRomBankReg));
    printf("GSU_RAMBANK_OFF   %zu\n", offsetof(struct FxRegs_s, vRamBankReg));
    printf("GSU_CACHEBASE_OFF %zu\n", offsetof(struct FxRegs_s, vCacheBaseReg));
    printf("GSU_CACHEFLAG_OFF %zu\n", offsetof(struct FxRegs_s, vCacheFlags));
    printf("GSU_LASTRAM_OFF   %zu\n", offsetof(struct FxRegs_s, vLastRamAdr));
    printf("GSU_DREG_OFF      %zu\n", offsetof(struct FxRegs_s, pvDreg));
    printf("GSU_SREG_OFF      %zu\n", offsetof(struct FxRegs_s, pvSreg));
    printf("GSU_ROMBUF_OFF    %zu\n", offsetof(struct FxRegs_s, vRomBuffer));
    printf("GSU_PIPE_OFF      %zu\n", offsetof(struct FxRegs_s, vPipe));
    printf("GSU_PIPEADR_OFF   %zu\n", offsetof(struct FxRegs_s, vPipeAdr));
    printf("GSU_VSIGN_OFF     %zu\n", offsetof(struct FxRegs_s, vSign));
    printf("GSU_VZERO_OFF     %zu\n", offsetof(struct FxRegs_s, vZero));
    printf("GSU_VCARRY_OFF    %zu\n", offsetof(struct FxRegs_s, vCarry));
    printf("GSU_VOVERFLOW_OFF %zu\n", offsetof(struct FxRegs_s, vOverflow));
    printf("GSU_PVREGISTERS_OFF %zu\n", offsetof(struct FxRegs_s, pvRegisters));
    printf("GSU_RAMBANK_PTR   %zu\n", offsetof(struct FxRegs_s, pvRamBank));
    printf("GSU_ROMBANK_PTR   %zu\n", offsetof(struct FxRegs_s, pvRomBank));
    printf("GSU_PBR_PTR       %zu\n", offsetof(struct FxRegs_s, pvPrgBank));
    printf("GSU_APVROMBANK_OFF %zu\n", offsetof(struct FxRegs_s, apvRomBank));
    printf("GSU_BCACHEACTIVE_OFF %zu\n", offsetof(struct FxRegs_s, bCacheActive));
    printf("GSU_VCOUNT_OFF    %zu\n", offsetof(struct FxRegs_s, vCounter));
    printf("GSU_VINSTCOUNT_OFF %zu\n", offsetof(struct FxRegs_s, vInstCount));
    return 0;
}
