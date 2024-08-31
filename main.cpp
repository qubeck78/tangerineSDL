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

   printf( "TangerineRiscVSOC emulator B20240831 -qUBECk78@wp.pl-\n\n" );

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
   if( srecLoadFile( ( char* )"boot.rec", &i ) )
   {
      printf( "Error, can't load bootloader. Ensure boot.rec file is in the same dir as emulator executable.\n" );
   }

   srecLoadFile( ( char* )"fractal.rec", &i );
   cpuctx.pc = i;


   do
   {

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

