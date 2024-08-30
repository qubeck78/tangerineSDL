
#include "tangerine.h"

#include <stdio.h>



ulong tgInit( tangerineCtx_t *ctx )
{
   FILE  *in;
   ulong i;

   ctx->window    = NULL;
   ctx->renderer  = NULL;
   ctx->texture   = NULL;
   ctx->dmaRAM    = NULL;
   ctx->fastRAM   = NULL;
   ctx->systemRAM = NULL;


   if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) 
   {
     printf( "Error: SDL_Init failed '%s'\n", SDL_GetError() );
     return RV_ERROR;
   }

   ctx->window = SDL_CreateWindow( "TangerineRiscVSOC simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_ALLOW_HIGHDPI );

   if( ctx->window == NULL ) 
   {
     printf( "Error: Could not create window '%s'\n", SDL_GetError() );
     return RV_ERROR;
   }  

   // create renderer
   ctx->renderer = SDL_CreateRenderer( ctx->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

   if( ctx->renderer == NULL ) 
   {
     printf( "Error: Unable to create renderer '%s'\n", SDL_GetError() );
     return RV_ERROR;
   }

   SDL_RenderSetLogicalSize( ctx->renderer, 640, 480 );

   ctx->texture = SDL_CreateTexture( ctx->renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 640, 480 );

   if( ctx->texture == NULL ) 
   {
     printf( "Error: Unable to create texture '%s'\n", SDL_GetError() );
     return RV_ERROR;
   }

   ctx->systemRAM = malloc( _TG_SYSTEM_RAM_SIZE );

   if( ctx->systemRAM == NULL )
   {
      printf( "Error: Unable to alloc 32K for systemRAM\n" );
      return RV_ERROR;

   }

   for( i = 160 * 5; i < 160 * 15; i++ )
   {
      ( (uchar*)ctx->systemRAM )[ 0x6d40 + i ] = rand();

   }

   ctx->dmaRAM = malloc( _TG_DMA_RAM_SIZE );

   if( ctx->dmaRAM == NULL )
   {
      printf( "Error: Unable to alloc %d bytes for dmaRAM\n", _TG_DMA_RAM_SIZE );
      return RV_ERROR;

   }

   for( i = 0; i < _TG_DMA_RAM_SIZE; i++ )
   {
      ( (uchar*)ctx->dmaRAM )[i] = rand();

   }

   ctx->fastRAM = malloc( _TG_FAST_RAM_SIZE );

   if( ctx->fastRAM == NULL )
   {
      printf( "Error: Unable to alloc %d bytes for fastRAM\n", _TG_FAST_RAM_SIZE );
      return RV_ERROR;

   }

   //load console font

   in = fopen( "font.dat", "rb" );
   if( !in )
   {
     printf(" Error: Can't load font.dat\n" );
     return RV_ERROR;
   }

   fread( ( void* )&ctx->consoleFont, 1, 2048, in );
   fclose( in );

   //init registers

   rootRegsInit( &ctx->rootRegs );
   sdramDMARegsInit( &ctx->sdramDMARegs );

   

   return RV_OK;        
}

ulong tgHandleEvents( tangerineCtx_t *ctx )
{
   SDL_Event    event;

   if( SDL_PollEvent( &event ) ) 
   {
      if( event.type == SDL_QUIT ) 
      {
         return RV_QUIT;
      }
   }



   return RV_OK;

}


ulong tgClose( tangerineCtx_t *ctx )
{

   if( ctx->texture != NULL )
   {
     SDL_DestroyTexture( ctx->texture );
     ctx->texture = NULL;

   }

   if( ctx->renderer != NULL )
   {
     SDL_DestroyRenderer( ctx->renderer );
     ctx->renderer = NULL;

   }

   if( ctx->window != NULL )
   {
     SDL_DestroyWindow( ctx->window );
     ctx->window = NULL;

   }


   if( ctx->systemRAM != NULL )
   {
      free( ctx->systemRAM );
      ctx->systemRAM = NULL;

   }

   if( ctx->dmaRAM != NULL )
   {
      free( ctx->dmaRAM );
      ctx->dmaRAM = NULL;

   }
   
   if( ctx->fastRAM != NULL )
   {
      free( ctx->fastRAM );
      ctx->fastRAM = NULL;

   }

   return RV_OK;
}

