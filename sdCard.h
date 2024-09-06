#ifndef _SDCARD_H
#define _SDCARD_H

#include "gfTypes.h"
#include <cstdio>

typedef enum   _sdcState_t { sdcsIdle, sdcsCMD0, sdcsCMD1, sdcsCMD2, sdcsCMD3, sdcsCMD4, sdcsCMD5, sdcsCMD6, sdcsSendResponseBuf }sdcState_t;

typedef struct _sdcContext_t
{
   
   sdcState_t   state;
   FILE        *imageFile;
   
   uint32_t        spiResponse;

   uint32_t        cmd;
   uint32_t        arg;
   uint32_t        crc;

   uint32_t sdCEn;

   uint8_t  responseBuf[1024];
   uint32_t responseIdx;
   uint32_t responseMaxIdx;

}sdcContext_t;

uint32_t sdcInit( sdcContext_t *ctx, char * imageFileName );

uint32_t sdcSPICE( sdcContext_t *ctx, uint32_t enabled );
uint32_t sdcSPIRead( sdcContext_t *ctx );
uint32_t sdcSPIWrite( sdcContext_t *ctx, uint32_t value );


#endif
