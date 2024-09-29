#include "audio.h"


//https://discourse.libsdl.org/t/how-to-make-audio-programatically-by-sdl2/49260/3

int16_t sampleBuffer[2048];

uint32_t audioInit( audContext_t *audCtx, void *pdmaRam )
{

   SDL_AudioSpec audioSpec;

   audioSpec.freq       = 48000;
   audioSpec.format     = AUDIO_S16SYS;
   audioSpec.channels   = 2;
   audioSpec.samples    = 1024;
   audioSpec.callback   = NULL;

   audCtx->audioDeviceId = SDL_OpenAudioDevice( NULL, 0, &audioSpec, NULL, 0 );

   if( !audCtx->audioDeviceId )
   {
      printf( "%s\n", SDL_GetError() );
      
      return RV_ERROR;
   }

   SDL_PauseAudioDevice( audCtx->audioDeviceId, 0 ); // start audio

   audCtx->dmaRam          = (int16_t*)pdmaRam;

   audCtx->freqDivision    = 0;
   audCtx->dmaMode         = 0;   
   audCtx->dmaPointer      = 0;
   audCtx->dmaIdx          = 0;
   audCtx->dmaTransferSize = 0;
   audCtx->dmaStatus       = 0;

   return RV_OK;
}

uint32_t audioSetDMAConfig( audContext_t *audCtx, uint32_t freqDivision, uint32_t dmaMode, uint32_t dmaPointer, uint32_t dmaTransferSize )
{
   
   //convert address to index of int16_t array
   dmaPointer &= 0x7fffff;
   dmaPointer >>= 1;

   //convert transfer size to index of int16_t array
   dmaTransferSize += 1;
   dmaTransferSize <<= 1;

   audCtx->freqDivision    = freqDivision;
   audCtx->dmaMode         = dmaMode & 7;
   audCtx->dmaPointer      = dmaPointer;
   audCtx->dmaTransferSize = dmaTransferSize;

   audCtx->dmaIdx          = 0;

   return RV_OK;
}

uint32_t audioGetDMAStatus( audContext_t *audCtx )
{
   return audCtx->dmaStatus;
}


uint32_t audioMain( audContext_t *audCtx )
{
   uint32_t i;
   uint32_t j;
   int16_t  l;
   int16_t  r;

   if( !SDL_GetQueuedAudioSize( audCtx->audioDeviceId ) )
   {

      switch( audCtx->dmaMode & 3 )
      {

         case 0x00:

            break;

         case 0x02:  

            //for( i = 0; i < 2048; i += 4 )
            i = 0;
            do
            {

               l = audCtx->dmaRam[ audCtx->dmaPointer + audCtx->dmaIdx++ ];
               r = audCtx->dmaRam[ audCtx->dmaPointer + audCtx->dmaIdx++ ];

               if( audCtx->dmaIdx >= audCtx->dmaTransferSize )
               {
                  audCtx->dmaIdx = 0;
               }

               if( audCtx->dmaIdx >= audCtx->dmaTransferSize / 2 )
               {
                  audCtx->dmaStatus |= 2;
               }
               else
               {
                  audCtx->dmaStatus &= ( 2 ^ 0xffffffff );            
               }
               
               for( j = 0; j <= audCtx->freqDivision; j++ )
               {
                  sampleBuffer[ i++ ] = l;
                  sampleBuffer[ i++ ] = r;
               }


            }while( i < 2048 );

            SDL_QueueAudio( audCtx->audioDeviceId, sampleBuffer, 2048 * 2  );

            break;
      }
   }

   return RV_OK;
}