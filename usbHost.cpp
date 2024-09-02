
#include "usbHost.h"

#include <cstdio>

ulong usbHostInit( usbhContext_t *ctx )
{
   //clear fifo

   ctx->usbhFifoNumEntries = 0;
   ctx->usbhFifoRdIdx      = 0;
   ctx->usbhFifoWrIdx      = 0;

   return RV_OK;
}

ulong usbHostFifoPut( usbhContext_t *ctx, ulong value )
{
   if( ctx->usbhFifoNumEntries < ( _USBHOST_FIFO_SIZE - 1 ) )
   {
      ctx->usbhFifoBuf[ctx->usbhFifoWrIdx++] = value;
      ctx->usbhFifoWrIdx &= ( _USBHOST_FIFO_SIZE - 1 );
      ctx->usbhFifoNumEntries++;

   }
   return RV_OK;
}

ulong usbHostFifoGet( usbhContext_t *ctx )
{
   ulong rv;

   rv = 0;

   if( ctx->usbhFifoNumEntries )
   {

      rv = ctx->usbhFifoBuf[ctx->usbhFifoRdIdx++];
      ctx->usbhFifoRdIdx &= ( _USBHOST_FIFO_SIZE - 1 );
      ctx->usbhFifoNumEntries--;

   }

   return rv;
}

ulong usbHostFifoEmpty( usbhContext_t *ctx )
{
   if( ctx->usbhFifoNumEntries )
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

ulong usbHostKeyDownEvent( usbhContext_t *ctx, ulong scancode )
{
   usbHostFifoPut( ctx, scancode );

   return RV_OK;
}

ulong usbHostKeyUpEvent( usbhContext_t *ctx, ulong scancode )
{
   usbHostFifoPut( ctx, 0x0 );

   return RV_OK;
}
