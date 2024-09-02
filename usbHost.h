#ifndef _USBHOST_H
#define _USBHOST_H

#include "gftypes.h"

#define _USBHOST_FIFO_SIZE 32

typedef struct _usbhContext_t
{

   ulong usbhFifoBuf[_USBHOST_FIFO_SIZE];
   ulong usbhFifoNumEntries;
   ulong usbhFifoRdIdx;
   ulong usbhFifoWrIdx;


}usbhContext_t;


ulong usbHostInit( usbhContext_t *ctx );

ulong usbHostFifoPut( usbhContext_t *ctx, ulong value );
ulong usbHostFifoGet( usbhContext_t *ctx );
ulong usbHostFifoEmpty( usbhContext_t *ctx );

ulong usbHostKeyDownEvent( usbhContext_t *ctx, ulong scancode );
ulong usbHostKeyUpEvent( usbhContext_t *ctx, ulong scancode );

#endif
