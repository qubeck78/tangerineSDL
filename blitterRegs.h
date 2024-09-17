#ifndef _BLITTERREGS_H
#define _BLITTERREGS_H

#include "gfTypes.h"


typedef struct _tgBlitterRegs_t
{

    volatile uint32_t id;
    volatile uint32_t version;

    volatile uint32_t command;
    volatile uint32_t input0;
    volatile uint32_t input1;
    volatile uint32_t input2;
    volatile uint32_t input3;
    volatile uint32_t saAddress;
    volatile uint32_t saRowWidth;
    volatile uint32_t saWidth;
    volatile uint32_t saHeight;
    volatile uint32_t sbAddress;
    volatile uint32_t sbRowWidth;
    volatile uint32_t scAddress;
    volatile uint32_t scRowWidth;
    volatile uint32_t daAddress;
    volatile uint32_t daRowWidth;
    volatile uint32_t daWidth;
    volatile uint32_t daHeight;
    volatile uint32_t dbAddress;
    volatile uint32_t dbRowWidth;


    uint32_t saColAdd;
    uint32_t sbColAdd;
    uint32_t scColAdd;
       
    uint32_t daColAdd;
    uint32_t dbColAdd;
       

}tgBlitterRegs_t;


uint32_t blitterRegsInit( tgBlitterRegs_t *regs );
uint32_t blitterRegsReadReg(  tgBlitterRegs_t *regs, uint16_t addr );
uint32_t blitterRegsWriteReg(  tgBlitterRegs_t *regs, uint16_t addr, uint32_t value );


#endif
