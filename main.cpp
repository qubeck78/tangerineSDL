
/*

   Tangerine Risc-V SOC emulator

   Copyright (c) 2024, Michał Kubecki - qubeck78@wp.pl

   Supplied under BSD-3 Clause license ( see LICENSE file )


   Note: While not enforced by the license, I kindly request that any application 
   using this software includes "Michal Kubecki - qubeck78@wp.pl" in its about 
   or credits section as a token of appreciation for the open-source contribution.

*/


#include "main.h"
#include <cstring>
#include <climits>

#include "tangerine.h"
#include "tgVideoOut.h"
#include "emul.h"
#include "memio.h"
#include "srec.h"
#include "sdCard.h"

tangerineCtx_t tgctx;
emContext_t    cpuctx;


int main( int argc,  char** argv )
{
   uint32_t i;
   uint32_t cpurv;

   printf( "TangerineRiscVSOC emulator B20240928 -qUBECk78@wp.pl-\n\n" );


   //memory access
   if( mioInit( &tgctx) )
   {

      return 1;

   }

   cpuctx.fetchInstruction = fetchInstruction;
   cpuctx.fetchData        = fetchData;
   cpuctx.storeData        = storeData;


   //sd card
   if( sdcInit( &tgctx.sdCardContext, (char*)"sdcardPrv.img" ) )
   {
      if( sdcInit( &tgctx.sdCardContext, (char*)"sdcard.img" ) )
      {
         printf( "Can't load sd card image. Ensure srcard.img is in the same dir as emulator executable.\n" );      
      }
   }

   //usb host
   if( usbHostInit( &tgctx.usbHostContext ) )
   {
      printf( "Error, can't init usb host emulation\n" );            
   }


   tgctx.displayFullscreen = 0;

   //hardware emulation
   if( tgInit( &tgctx ) )
   {
      
      tgClose( &tgctx );;
      return 1;

   }

   //audio, has to be called after tgInit because it depends on SDL
   
   if( audioInit( &tgctx.audioContext ) )
   {
      printf( "Error, can't init audio\n" );            
   }

   rvReset( &cpuctx );
   if( srecLoadFile( ( char* )"boot.rec", &i ) )
   {
      printf( "Error, can't load bootloader. Ensure boot.rec file is in the same dir as emulator executable.\n" );
   }

   if( argc > 1 )
   {
      printf( "Loading: \"%s\"\n", argv[1] );
      if( srecLoadFile( argv[1], &i ) )
      {
         printf( "error\n" );
      }
      else
      {
         cpuctx.pc = i;
      }
   }
   else
   {
      printf( "Error: No app to load given - running default bootloader.\nusage: tangerine program.rec\n" );
   }

   do
   {

      for( i = 0; i < 1000000; i++ )
      {
         cpurv = rvStep( &cpuctx );
         
         if( cpurv )
         {
            break;
         }

      }

      tgRedrawScreen( &tgctx );

      tgctx.rootRegs.frameTimer++;
      tgctx.rootRegs.videoVSync = 1;   //sim vsync

      audioMain( &tgctx.audioContext );

      if( tgHandleEvents( &tgctx ) == RV_QUIT )
      {
         break;
      }

   }while( 1 );




   tgClose( &tgctx );

   return 0;
} 

