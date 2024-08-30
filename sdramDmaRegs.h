#ifndef _SDRAMDMA_H
#define _SDRAMDMA_H

#include "gftypes.h"

typedef struct _tgSDRAMDMARegs_t
{
    volatile ulong id;
    volatile ulong version;
    volatile ulong ch3DmaPointerStart;
    volatile ulong ch3DmaRequest0Modulo;
    volatile ulong ch3DmaRequest1Modulo;

}tgSDRAMDMARegs_t;


ulong sdramDMARegsInit( tgSDRAMDMARegs_t *regs );
ulong sdramDMARegsReadReg(  tgSDRAMDMARegs_t *regs, ushort addr );
ulong sdramDMARegsWriteReg(  tgSDRAMDMARegs_t *regs, ushort addr, ulong value );


#endif
