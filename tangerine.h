#ifndef _TANGERINE_H
#define _TANGERINE_H

#include "gfTypes.h"
#include "emul.h"
#include "sdramDmaRegs.h"
#include "rootRegs.h"
#include "spiSdCardRegs.h"
#include "usbHostRegs.h"
#include "blitterRegs.h"
#include "sdCard.h"
#include "usbHost.h"
#include "audioRegs.h"
#include "audio.h"

#include <SDL2/SDL.h>

//64 MB DMA ram
#define _TG_DMA_RAM_SIZE      ( 1048576 * 64 )

//1 MB FAST ram
#define _TG_FAST_RAM_SIZE     ( 1048576 * 1 )

//32K SYSTEM ram
#define _TG_SYSTEM_RAM_SIZE   32768

#define _VIDEOMODE_TEXT40_ONLY              0x00
#define _VIDEOMODE_TEXT80_ONLY              0x04
#define _VIDEOMODE_TEXT80_60_ONLY           0x0c

#define _VIDEOMODE_320_TEXT40_OVER_GFX      0x02
#define _VIDEOMODE_320_TEXT80_OVER_GFX      0x06
#define _VIDEOMODE_320_TEXT80_60_OVER_GFX   0x0e

#define _VIDEOMODE_640_TEXT40_OVER_GFX      0x12
#define _VIDEOMODE_640_TEXT80_OVER_GFX      0x16
#define _VIDEOMODE_640_TEXT80_60_OVER_GFX   0x1e

#define tgColor565( r, g, b ) ( uint16_t )((((uint16_t)b >> 3) & 31 ) | (((uint16_t)g & 252 ) << 3 ) | (((uint16_t)r & 248 ) << 8 ))
#define tgColor565GetR( color ) ( uint16_t )(( color >> 8) & 0xf8 )
#define tgColor565GetG( color ) ( uint16_t )(( color >> 3) & 0xfc )
#define tgColor565GetB( color ) ( uint16_t )(( color << 3) & 0xf8 )

typedef struct _tangerineCtx_t
{

   SDL_Window        *window;
   SDL_Renderer      *renderer;
   SDL_Texture       *texture;
   uint32_t           displayFullscreen;
   uint32_t           exitMainLoop;
   uint32_t           debuggerActive;
   
   void              *dmaRAM;
   void              *fastRAM;
   void              *systemRAM;

   uint8_t            consoleFont[2048];

   emContext_t        cpuctx;
   
   tgRootRegs_t       rootRegs;
   tgBlitterRegs_t    blitterRegs;
   tgSDRAMDMARegs_t   sdramDMARegs;
   tgSpiSDCardRegs_t  spiSdCardRegs;
   tgUsbHostRegs_t    usbHostRegs;
   tgAudioRegs_t      audioRegs;
   
   sdcContext_t       sdCardContext;
   usbhContext_t      usbHostContext;
   audContext_t       audioContext;


}tangerineCtx_t;


uint32_t tgInit( tangerineCtx_t *ctx );
uint32_t tgClose( tangerineCtx_t *ctx );
uint32_t tgHandleEvents( tangerineCtx_t *ctx );


#endif

