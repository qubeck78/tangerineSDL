#ifndef _AUDIOREGS_H
#define _AUDIOREGS_H

#include "gfTypes.h"
#include "audio.h"

typedef struct _tgAudioRegs_t
{

    volatile uint32_t id;
    volatile uint32_t version;
    volatile uint32_t audioFiFoData;
    volatile uint32_t audioFiFoStatus;
    volatile uint32_t i2sClockConfig;
    volatile uint32_t fifoReadConfig;
    volatile uint32_t audioDmaConfig;
    volatile uint32_t audioDmaStatus;
    volatile uint32_t audioDmaPointer;
    volatile uint32_t audioDmaLength;

}tgAudioRegs_t;


uint32_t audioRegsInit( tgAudioRegs_t *regs );
uint32_t audioRegsReadReg(  tgAudioRegs_t *regs, audContext_t *audCtx, uint16_t addr );
uint32_t audioRegsWriteReg(  tgAudioRegs_t *regs, audContext_t *audCtx, uint16_t addr, uint32_t value );


#endif
