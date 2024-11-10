#ifndef _ROOTREGS_H
#define _ROOTREGS_H

#include "gfTypes.h"

#include "sdCard.h"

typedef struct _tgRootRegs_t
{
    volatile uint32_t   id;
    volatile uint32_t   version;

    //
    volatile uint32_t   videoMuxMode;
    
    //b0 - vsync (positive)
    volatile uint32_t   videoVSync;
    
    volatile uint32_t   unused0;
        
    //wr b7, b6, b5, b4 - LEDS, b0 - spi0SSel
    volatile uint32_t   gpoPort;
    
    //wr b0 - tickTimerReset
    volatile uint32_t   tickTimerConfig;
    
    //rd - tickTimerValue
    volatile uint32_t   tickTimerValue;
    
    //counts frames (up), write resets timer
    volatile uint32_t   frameTimer;

    //counts cpu cycles
    volatile uint64_t   mtime;

    //compares cpu cycles ( for irq )
    volatile uint64_t   mtimeCmp;


}tgRootRegs_t;


uint32_t rootRegsInit( tgRootRegs_t *regs );
uint32_t rootRegsReadReg( tgRootRegs_t *regs, uint16_t addr );
uint32_t rootRegsWriteReg( tgRootRegs_t *regs, sdcContext_t *sdctx, uint16_t addr, uint32_t value );


#endif
