#include "debugger.h"

#include <stdio.h>
#include <string.h>

#include "memio.h"

static int32_t dbgLeave( debugCtx_t *ctx )
{

   //do not call debugger in main loop
   ctx->tgCtx->debuggerActive = 0;

   //restore videomux
   storeData( 0xf0000008, 3, ctx->tgVMMode );

   return 0;   
}


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

static int32_t dbgDumpRegisters( debugCtx_t *ctx )
{
   char     buf[256];
   uint32_t i;

   ctx->con.textAttributes = 0x8f;

   conSetCursorPos( &ctx->con, 0, 0 );
   conPrintF( &ctx->con, (char*)"PC:%08x", ctx->tgCtx->cpuctx.pc );

   for( i = 1; i < 32; i++ )
   {
      conSetCursorPos( &ctx->con, ( i % 6 ) * 13, i / 6 );
      nameRegister( i, buf );
      conPrintF( &ctx->con, (char*)"%s:%08x", buf, ctx->tgCtx->cpuctx.regs[i] );
   }

   return 0;
}

static int32_t dbgDisassemble( debugCtx_t *ctx )
{
   char     buf[256];
   uint32_t i;

   conSetCursorPos( &ctx->con, 0, 7 );

   ctx->dsCtx.codeBufIdx = -11;
   for( i = 0; i < 22; i++ )
   {

      dsDisassemble( &ctx->dsCtx, buf );
      if( ctx->dsCtx.codeBufIdx == 0 )
      {
         ctx->con.textAttributes = 0x8c;
      }
      else
      {
         ctx->con.textAttributes = 0x8f;
      }

      conPrintF( &ctx->con, (char*)"%08x    %s\n", ( ctx->dsCtx.codeBufStartPc + ctx->dsCtx.codeBufIdx * 4 ), buf );

      ctx->dsCtx.codeBufIdx++;

   }

   return 0;
}

static int32_t dbgMenu( debugCtx_t *ctx )
{
   conSetCursorPos( &ctx->con, 0, 29 );
   ctx->con.textAttributes = 0x2f;
   conPrintF( &ctx->con, (char*)"SPACE" );
   ctx->con.textAttributes = 0x8f;
   conPrintF( &ctx->con, (char*)" - step " );

   ctx->con.textAttributes = 0x2f;
   conPrintF( &ctx->con, (char*)"ENTER" );
   ctx->con.textAttributes = 0x8f;
   conPrintF( &ctx->con, (char*)" - run" );

   return 0;
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
      
   }

   ctx->con.textAttributes = 0x0f;
   conCls( &ctx->con );

   dbgDumpRegisters( ctx );

   ctx->dsCtx.codeBufStartPc  = ctx->tgCtx->cpuctx.pc;

   dbgDisassemble( ctx );

   dbgMenu( ctx );

   if( SDL_PollEvent( &event ) ) 
   {
      if( event.type == SDL_QUIT ) 
      {
         return RV_QUIT;

      }else if( event.type == SDL_KEYDOWN )
      {


         switch( event.key.keysym.sym )
         {

            case SDLK_SPACE:

               rvStep( &ctx->tgCtx->cpuctx );
               
               break;

            case SDLK_ESCAPE:
            case SDLK_RETURN:

               dbgLeave( ctx );

               break;

         }
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