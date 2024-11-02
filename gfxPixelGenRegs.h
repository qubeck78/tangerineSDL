#ifndef _GFXPIXELGENREGS_H
#define _GFXPIXELGENREGS_H

#include "gfTypes.h"

typedef struct _tgGfxPixelGenRegs_t
{

    volatile uint32_t id;
    volatile uint32_t version;

    volatile uint32_t palette[256];

}tgGfxPixelGenRegs_t;


uint32_t gfxPixelGenRegsInit( tgGfxPixelGenRegs_t *regs );
uint32_t gfxPixelGenRegsReadReg(  tgGfxPixelGenRegs_t *regs, uint32_t addr );
uint32_t gfxPixelGenRegsWriteReg(  tgGfxPixelGenRegs_t *regs, uint32_t addr, uint32_t value );


#endif
