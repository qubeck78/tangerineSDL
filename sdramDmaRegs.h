#ifndef _SDRAMDMA_H
#define _SDRAMDMA_H

#include "gfTypes.h"

typedef struct _tgSDRAMDMARegs_t
{
    volatile uint32_t id;
    volatile uint32_t version;
    volatile uint32_t ch3DmaPointerStart;
    volatile uint32_t ch3DmaRequest0Modulo;
    volatile uint32_t ch3DmaRequest1Modulo;

}tgSDRAMDMARegs_t;


uint32_t sdramDMARegsInit( tgSDRAMDMARegs_t *regs );
uint32_t sdramDMARegsReadReg(  tgSDRAMDMARegs_t *regs, uint16_t addr );
uint32_t sdramDMARegsWriteReg(  tgSDRAMDMARegs_t *regs, uint16_t addr, uint32_t value );


#endif
