#ifndef _MEMIO_H
#define _MEMIO_H

#include "gftypes.h"
#include "tangerine.h"

ulong mioInit( tangerineCtx_t *ptgctx );

ulong fetchInstruction( ulong addr );
ulong fetchData( ulong addr );
ulong storeData( ulong addr, uchar mask, ulong data );

#endif
