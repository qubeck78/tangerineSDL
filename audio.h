#ifndef _AUDIO_H
#define _AUDIO_H

#include "gfTypes.h"

#include <SDL2/SDL.h>

typedef struct _audContext_t
{
   SDL_AudioDeviceID audioDeviceId;

}audContext_t;


uint32_t audioInit( audContext_t *audCtx );
uint32_t audioMain( audContext_t *audCtx );
#endif
