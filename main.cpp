
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
#include "disasm.h"
#include "debugger.h"

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <emscripten/html5.h>

#endif

tangerineCtx_t tgctx;
debugCtx_t     dbgctx;

void mainLoop()
{
   uint32_t cpurv;
   uint32_t i;
   uint32_t j;

   if( tgctx.debuggerActive )
   {

      
      if( dbgMain( &dbgctx ) == RV_QUIT )
      {

         #ifdef __EMSCRIPTEN__

            emscripten_cancel_main_loop();

         #else

            tgctx.exitMainLoop = 1;

         #endif
      }   

      tgRedrawScreen( &tgctx );
      //audioMain( &tgctx.audioContext );

   }
   else
   {

      for( j = 0; j < 1000; j++ )
      {
         for( i = 0; i < 1000; i++ )
         {
            cpurv = rvStep( &tgctx.cpuctx );
            
            if( cpurv )
            {
               tgctx.debuggerActive = 1;  //init & run debugger
               break;
            }

         }

         tgctx.rootRegs.mtime += 1000;
         
      }
      tgRedrawScreen( &tgctx );

      tgctx.rootRegs.frameTimer++;
      tgctx.rootRegs.videoVSync = 1;   //sim vsync

      audioMain( &tgctx.audioContext );

      if( tgHandleEvents( &tgctx ) == RV_QUIT )
      {

         #ifdef __EMSCRIPTEN__

            emscripten_cancel_main_loop();

         #else

            tgctx.exitMainLoop = 1;

         #endif
      }   
   }
}


int main( int argc,  char** argv )
{
   uint32_t i;


   printf( "TangerineRiscVSOC emulator B20241102 -qUBECk78@wp.pl-\n\n" );


   //memory access
   if( mioInit( &tgctx) )
   {

      return 1;

   }

   tgctx.cpuctx.fetchInstruction = fetchInstruction;
   tgctx.cpuctx.fetchData        = fetchData;
   tgctx.cpuctx.storeData        = storeData;


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
      
      tgClose( &tgctx );
      return 1;

   }

   //audio, has to be called after tgInit because it depends on SDL
   
   if( audioInit( &tgctx.audioContext, tgctx.dmaRAM ) )
   {
      printf( "Error, can't init audio\n" );            
   }

   rvReset( &tgctx.cpuctx );
   if( srecLoadFile( ( char* )"boot.rec", &i ) )
   {
      printf( "Error, can't load bootloader. Ensure boot.rec file is in the same dir as emulator executable.\n" );
   }

   if( argc > 1 )
   {
      printf( "Loading: \"%s\" ", argv[1] );
      if( srecLoadFile( argv[1], &i ) )
      {
         printf( "error\n" );
      }
      else
      {
         tgctx.cpuctx.pc = i;
         printf( "ok\n" );
      }
   }
   else
   {
      printf( "Error: No app to load given - running default bootloader.\nusage: tangerine program.rec\n" );
   }

   printf( "\n" );

   if( dbgInit( &dbgctx, &tgctx ) )
   {
      printf( "Error, can't init debugger\n" );
   }
   else
   {
      printf( "Press F12 to activate debugger\n" );
   }

   tgctx.exitMainLoop = 0;

   do
   {

      #ifdef __EMSCRIPTEN__
      
         emscripten_set_main_loop( mainLoop, 0,  1 );

      #else
         
         mainLoop();


      #endif

   }while( !tgctx.exitMainLoop );



   tgClose( &tgctx );

   return 0;
} 

