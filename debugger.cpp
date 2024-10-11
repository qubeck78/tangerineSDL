#include "debugger.h"

#include <stdio.h>
#include <string.h>

#include "memio.h"

int32_t dbgInit( debugCtx_t *ctx, tangerineCtx_t *tgCtx )
{
   int32_t rv;

   if( ctx == NULL )
   {
      return 1;
   }

   if( tgCtx == NULL )
   {
      return 1;
   }

   ctx->tgCtx = tgCtx;

   rv = conInit( tgCtx, &ctx->con );

   ctx->dsCtx.codeBufIdx      = 0;
   ctx->dsCtx.codeBufStartPc  = 0;

   ctx->tgVMMode              = 0;
   
   return rv;
}

int32_t dbgMain( debugCtx_t *ctx )
{
   SDL_Event   event;

   if( ctx->tgCtx->debuggerActive == 0 )
   {
      return 0;
   }

   if( ctx->tgCtx->debuggerActive == 1 )
   {
      //init debugger screen

      //save videoMux mode
      ctx->tgVMMode = fetchData( 0xf0000008 );  //videoMux reg

      //set videoMux to text 80x60, over gfx
      storeData( 0xf0000008, 3, ( ctx->tgVMMode & 0xf0 ) | 0x0e );

      ctx->tgCtx->debuggerActive = 2;

      ctx->con.textAttributes = 0x0f;
      conCls( &ctx->con );
      ctx->con.textAttributes = 0x8c;
      conPrintF( &ctx->con, (char*)"Debugger :D\n");
      
   }



   if( SDL_PollEvent( &event ) ) 
   {
      if( event.type == SDL_QUIT ) 
      {
         return RV_QUIT;

      }else if( event.type == SDL_KEYDOWN )
      {

         //do not call debugger in main loop
         ctx->tgCtx->debuggerActive = 0;

         //restore videomux
         storeData( 0xf0000008, 3, ctx->tgVMMode );

         //check if F12 pressed ( to invoke debugger )
/*         if( event.key.keysym.sym == SDLK_F12 )
         {
            ctx->debuggerActive = 1;
         }
*/
      }
   }

   return 0;
}