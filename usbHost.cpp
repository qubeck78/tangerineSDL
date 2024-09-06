
#include "usbHost.h"

#include <cstdio>

uint32_t usbHostInit( usbhContext_t *ctx )
{
   //clear fifo

   ctx->usbhFifoNumEntries = 0;
   ctx->usbhFifoRdIdx      = 0;
   ctx->usbhFifoWrIdx      = 0;

   return RV_OK;
}

uint32_t usbHostFifoPut( usbhContext_t *ctx, uint32_t value )
{
   if( ctx->usbhFifoNumEntries < ( _USBHOST_FIFO_SIZE - 1 ) )
   {
      ctx->usbhFifoBuf[ctx->usbhFifoWrIdx++] = value;
      ctx->usbhFifoWrIdx &= ( _USBHOST_FIFO_SIZE - 1 );
      ctx->usbhFifoNumEntries++;

   }
   return RV_OK;
}

uint32_t usbHostFifoGet( usbhContext_t *ctx )
{
   uint32_t rv;

   rv = 0;

   if( ctx->usbhFifoNumEntries )
   {

      rv = ctx->usbhFifoBuf[ctx->usbhFifoRdIdx++];
      ctx->usbhFifoRdIdx &= ( _USBHOST_FIFO_SIZE - 1 );
      ctx->usbhFifoNumEntries--;

   }

   return rv;
}

uint32_t usbHostFifoEmpty( usbhContext_t *ctx )
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

uint32_t usbHostKeyDownEvent( usbhContext_t *ctx, uint32_t scancode )
{
   usbHostFifoPut( ctx, scancode );

   return RV_OK;
}

uint32_t usbHostKeyUpEvent( usbhContext_t *ctx, uint32_t scancode )
{
   usbHostFifoPut( ctx, 0x0 );

   return RV_OK;
}
