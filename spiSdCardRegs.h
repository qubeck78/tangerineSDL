#ifndef _SPISDCARDREGS_H
#define _SPISDCARDREGS_H

#include "gfTypes.h"

#include "sdCard.h"

typedef struct _tgSpiSDCardRegs_t
{

    volatile uint32_t id;
    volatile uint32_t version;

    //wr - send data via spi, rd - read received data
    volatile uint32_t spiData;
    
    //rd b0 - spiReady
    volatile uint32_t spiStatus;

}tgSpiSDCardRegs_t;


uint32_t spiSdCardRegsInit( tgSpiSDCardRegs_t *regs );
uint32_t spiSdCardRegsReadReg(  tgSpiSDCardRegs_t *regs, sdcContext_t *sdctx, uint16_t addr );
uint32_t spiSdCardRegsWriteReg(  tgSpiSDCardRegs_t *regs, sdcContext_t *sdctx, uint16_t addr, uint32_t value );


#endif
