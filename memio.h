#ifndef _MEMIO_H
#define _MEMIO_H

#include "gfTypes.h"
#include "tangerine.h"

uint32_t mioInit( tangerineCtx_t *ptgctx );

uint32_t fetchInstruction( uint32_t addr );
uint32_t fetchData( uint32_t addr );
uint32_t storeData( uint32_t addr, uint8_t mask, uint32_t data );

#endif
