#ifndef _AUDIO_H
#define _AUDIO_H

#include "gfTypes.h"

#include <SDL2/SDL.h>

typedef struct _audContext_t
{
   SDL_AudioDeviceID  audioDeviceId;

   int16_t           *dmaRam;
   
   uint32_t           freqDivision;
   uint32_t           dmaMode;
   uint32_t           dmaPointer;
   uint32_t           dmaIdx;
   uint32_t           dmaTransferSize;

   uint32_t           dmaStatus;

}audContext_t;


uint32_t audioInit( audContext_t *audCtx, void *pdmaRam );
uint32_t audioMain( audContext_t *audCtx );

uint32_t audioSetDMAConfig( audContext_t *audCtx, uint32_t freqDivision, uint32_t dmaMode, uint32_t dmaPointer, uint32_t dmaTransferSize );
uint32_t audioGetDMAStatus( audContext_t *audCtx );


#endif
