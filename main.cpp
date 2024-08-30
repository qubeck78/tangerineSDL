#include "main.h"
#include <cstring>
#include <climits>

#include "tangerine.h"
#include "tgVideoOut.h"
#include "emul.h"
#include "memio.h"
#include "srec.h"

tangerineCtx_t tgctx;
emContext_t    cpuctx;


int main( int argc,  char** argv )
{
   ulong i;

   printf( "TangerineRiscVSOC emulator B20240830 -qUBECk-\n\n" );

   //hardware emulation

   //memory access
   if( mioInit( &tgctx) )
   {

      tgClose( &tgctx );
      return 1;

   }

   if( tgInit( &tgctx ) )
   {
      
      tgClose( &tgctx );
      return 1;

   }


   cpuctx.fetchInstruction = fetchInstruction;
   cpuctx.fetchData        = fetchData;
   cpuctx.storeData        = storeData;


   rvReset( &cpuctx );

   srecLoadFile( ( char* )"fractal.rec" );

   //cpuctx.pc = 0x2000;
   cpuctx.pc = 0x20780000;
   //cpuctx.pc = 0x30000010;

   do
   {

      //simulate 60fps refresh
//      SDL_Delay( 1000 / 60 );

      for( i = 0; i < 1000000; i++ )
      {
         rvStep( &cpuctx );
      }


      tgRedrawScreen( &tgctx );


      if( tgHandleEvents( &tgctx ) == RV_QUIT )
      {
         break;
      }

   }while( 1 );




   tgClose( &tgctx );

   return 0;
} 

