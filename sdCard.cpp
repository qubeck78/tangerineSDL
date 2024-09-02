
#include "sdCard.h"


ulong sdcInit( sdcContext_t *ctx, char * imageFileName )
{

   ctx->sdCEn        = 1;   //deselected
   ctx->spiResponse  = 0;

   ctx->imageFile    = fopen( imageFileName, "rb+" );
   
   if( !ctx->imageFile )
   {
      return RV_ERROR;
   }

   return RV_OK;
}

ulong sdcSPICE( sdcContext_t *ctx, ulong enabled )
{

   if( enabled == 1 )
   {
      //deselect
      ctx->state = sdcsIdle;

   }

   ctx->sdCEn = enabled;

   return RV_OK;
}

ulong sdcSPIRead( sdcContext_t *ctx )
{

   return ctx->spiResponse;
}

ulong sdcSPIWrite( sdcContext_t *ctx, ulong value )
{
   ulong i;

   if( ctx->sdCEn )
   {
      //deselected

      return RV_OK;
   }

   value &= 0xff;

   switch( ctx->state )
   {
      case sdcsIdle:

         ctx->spiResponse  = 0xff;  //card ready
         ctx->state        = sdcsCMD0;
         break;

      case sdcsCMD0:

         ctx->cmd          = value;

         ctx->spiResponse  = 0xff;
         ctx->state        = sdcsCMD1;

         //printf( "#SD CMD:%x\n", ctx->cmd );
         break;

      case sdcsCMD1:

         ctx->arg          = value << 24;

         ctx->spiResponse  = 0xff;
         ctx->state        = sdcsCMD2;
         break;

      case sdcsCMD2:

         ctx->arg          |= value << 16;

         ctx->spiResponse  = 0xff;
         ctx->state        = sdcsCMD3;
         break;

      case sdcsCMD3:

         ctx->arg          |= value << 8;

         ctx->spiResponse  = 0xff;
         ctx->state        = sdcsCMD4;
         break;

      case sdcsCMD4:

         ctx->arg          |= value;

         ctx->spiResponse  = 0xff;
         ctx->state        = sdcsCMD5;
         break;

      case sdcsCMD5:

         ctx->crc          = value;
         ctx->state        = sdcsCMD6;
         break;

      case sdcsCMD6:

         ctx->spiResponse  = 1;

         switch( ctx->cmd )
         {
            case 0x40:  //CMD0 - idle

               ctx->spiResponse  = 1;
               break;

            case 0x48: //CMD8

               ctx->responseMaxIdx                    = 0;
               ctx->responseIdx                       = 0;

               ctx->responseBuf[ctx->responseMaxIdx++] = 0x00;
               ctx->responseBuf[ctx->responseMaxIdx++] = 0x00;
               ctx->responseBuf[ctx->responseMaxIdx++] = 0x01;
               ctx->responseBuf[ctx->responseMaxIdx++] = 0xaa;

               ctx->state        = sdcsSendResponseBuf;

               break;

            case 0x77:  //CMD55 ( ACMD prefix)

               ctx->spiResponse = 0;
               break;

            case 0x69:  //ACMD41

               ctx->spiResponse  = 0;

               break;

            case 0x7a: //CMD58


               ctx->spiResponse  = 0;

               ctx->responseMaxIdx                    = 0;
               ctx->responseIdx                       = 0;

               ctx->responseBuf[ctx->responseMaxIdx++] = 0x40; //Block device
               ctx->responseBuf[ctx->responseMaxIdx++] = 0x00;
               ctx->responseBuf[ctx->responseMaxIdx++] = 0x00;
               ctx->responseBuf[ctx->responseMaxIdx++] = 0x00;

               ctx->state        = sdcsSendResponseBuf;
               break;

            case 0x51:  //CMD17

               ctx->spiResponse  = 0;

               ctx->responseMaxIdx                    = 0;
               ctx->responseIdx                       = 0;

               ctx->responseBuf[ctx->responseMaxIdx++] = 0xfe; //data block

               fseek( ctx->imageFile, ctx->arg * 512, SEEK_SET );

               fread( &ctx->responseBuf[ctx->responseMaxIdx], 512, 1, ctx->imageFile );

               ctx->responseMaxIdx += 512;


               ctx->responseBuf[ctx->responseMaxIdx++] = 0x00; //crc
               ctx->responseBuf[ctx->responseMaxIdx++] = 0x00;

               ctx->state        = sdcsSendResponseBuf;
               break;

            default:

               printf( "SD CMD:%x, %x\n", ctx->cmd, ctx->arg );
               break;
         }

         break;

      case sdcsSendResponseBuf:

         if( ctx->responseIdx < ctx->responseMaxIdx )
         {
            ctx->spiResponse = ctx->responseBuf[ ctx->responseIdx++ ];
         }
         else
         {
            ctx->spiResponse = 0;
         }

         break;
   }

   return RV_OK;
}

