#ifndef _ROOTREGS_H
#define _ROOTREGS_H

#include "gftypes.h"

#include "sdCard.h"

typedef struct _tgRootRegs_t
{
    volatile ulong id;
    volatile ulong version;

    //
    volatile unsigned long videoMuxMode;
    
    //b0 - vsync (positive)
    volatile unsigned long videoVSync;
    
    volatile unsigned long  unused0;
        
    //wr b7, b6, b5, b4 - LEDS, b0 - spi0SSel
    volatile unsigned long gpoPort;
    
    //wr b0 - tickTimerReset
    volatile unsigned long tickTimerConfig;
    
    //rd - tickTimerValue
    volatile unsigned long tickTimerValue;
    
    //counts frames (up), write resets timer
    volatile unsigned long  frameTimer;

}tgRootRegs_t;


ulong rootRegsInit( tgRootRegs_t *regs );
ulong rootRegsReadReg(  tgRootRegs_t *regs, ushort addr );
ulong rootRegsWriteReg(  tgRootRegs_t *regs, sdcContext_t *sdctx, ushort addr, ulong value );


#endif
