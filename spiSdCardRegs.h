#ifndef _SPISDCARDREGS_H
#define _SPISDCARDREGS_H

#include "gftypes.h"

typedef struct _tgSpiSDCardRegs_t
{

    volatile ulong id;
    volatile ulong version;

    //wr - send data via spi, rd - read received data
    volatile unsigned long spiData;
    
    //rd b0 - spiReady
    volatile unsigned long spiStatus;

}tgSpiSDCardRegs_t;


ulong spiSdCardRegsInit( tgSpiSDCardRegs_t *regs );
ulong spiSdCardRegsReadReg(  tgSpiSDCardRegs_t *regs, ushort addr );
ulong spiSdCardRegsWriteReg(  tgSpiSDCardRegs_t *regs, ushort addr, ulong value );


#endif
