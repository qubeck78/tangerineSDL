#ifndef _DEBUGGER_H
#define _DEBUGGER_H


#include "gfTypes.h"
#include "disasm.h"
#include "tangerine.h"
#include "console.h"

#include <SDL2/SDL.h>

typedef struct _debugCtx_t
{
   tangerineCtx_t *tgCtx;
   tdbgConsole     con;
   dsContext_t     dsCtx;

   uint32_t        tgVMMode;

}debugCtx_t;

int32_t dbgInit( debugCtx_t *ctx, tangerineCtx_t *tgCtx );
int32_t dbgMain( debugCtx_t *ctx );

#endif
