#ifndef _SDCARD_H
#define _SDCARD_H

#include "gftypes.h"
#include <cstdio>

typedef enum   _sdcState_t { sdcsIdle, sdcsCMD0, sdcsCMD1, sdcsCMD2, sdcsCMD3, sdcsCMD4, sdcsCMD5, sdcsCMD6, sdcsSendResponseBuf }sdcState_t;

typedef struct _sdcContext_t
{
   
   sdcState_t   state;
   FILE        *imageFile;
   
   ulong        spiResponse;

   ulong        cmd;
   ulong        arg;
   ulong        crc;

   ulong sdCEn;

   uchar responseBuf[1024];
   ulong responseIdx;
   ulong responseMaxIdx;

}sdcContext_t;

ulong sdcInit( sdcContext_t *ctx, char * imageFileName );

ulong sdcSPICE( sdcContext_t *ctx, ulong enabled );
ulong sdcSPIRead( sdcContext_t *ctx );
ulong sdcSPIWrite( sdcContext_t *ctx, ulong value );


#endif
