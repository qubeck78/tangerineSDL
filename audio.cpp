#include "audio.h"


//https://discourse.libsdl.org/t/how-to-make-audio-programatically-by-sdl2/49260/3

int16_t sampleBuffer[2048];

uint32_t audioInit( audContext_t *audCtx )
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

   return RV_OK;
}

uint32_t audioMain( audContext_t *audCtx )
{
   /*
   static uint32_t alpha = 0;
   uint32_t i;

   if( !SDL_GetQueuedAudioSize( audCtx->audioDeviceId ) )
   {

      for( i = 0; i < 2048; i += 2 )
      {
         sampleBuffer[ i ] = sin( alpha * ( 3.14 / 180.0 ) ) * 4096;
         sampleBuffer[ i + 1 ] = sin( ( alpha >> 2 ) * ( 3.14 / 180.0 ) ) * 4096;

         alpha += 6;

      }

      SDL_QueueAudio( audCtx->audioDeviceId, sampleBuffer, 2048 * 2  );

   }
   */
   return RV_OK;
}