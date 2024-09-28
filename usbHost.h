#ifndef _USBHOST_H
#define _USBHOST_H

#include "gfTypes.h"

#define _USBHOST_FIFO_SIZE 32

typedef struct _usbhContext_t
{

   uint32_t usbhFifoBuf[_USBHOST_FIFO_SIZE];
   uint32_t usbhFifoNumEntries;
   uint32_t usbhFifoRdIdx;
   uint32_t usbhFifoWrIdx;


}usbhContext_t;


uint32_t usbHostInit( usbhContext_t *ctx );

uint32_t usbHostFifoPut( usbhContext_t *ctx, uint32_t value );
uint32_t usbHostFifoGet( usbhContext_t *ctx );
uint32_t usbHostFifoEmpty( usbhContext_t *ctx );

uint32_t usbHostKeyDownEvent( usbhContext_t *ctx, uint32_t scancode );
uint32_t usbHostKeyUpEvent( usbhContext_t *ctx, uint32_t scancode );

#endif
