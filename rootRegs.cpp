#include "rootRegs.h"

#include <SDL2/SDL.h>

ulong rootRegsInit( tgRootRegs_t *regs )
{

   regs->videoMuxMode   = 0x0;
   regs->id             = 0x80000000;
   regs->version        = 0x20240828;
   regs->videoVSync     = 1;
   
   regs->tickTimerValue = SDL_GetTicks();
   regs->frameTimer     = 0;

   return RV_OK;

}

ulong rootRegsReadReg(  tgRootRegs_t *regs, ushort addr )
{
   switch( addr >> 2 )
   {
      case 0x00:

         return regs->id;

         break;

      case 0x01:

         return regs->version;

         break;

      case 0x02:

         return regs->videoMuxMode;

         break;

      case 0x03:

         return regs->videoVSync;

         break;

      case 0x04:

         return 0;

      case 0x05:

         return regs->gpoPort;

         break;

      case 0x06:

         //tick timer config

         return 0;
         
         break;

      case 0x07:

         //tick timer

         return (ulong)( SDL_GetTicks() - regs->tickTimerValue );
         
         break;

      case 0x08:

         //frame timer

         return regs->frameTimer;
         
         break;
   }

   return 0;
}

ulong rootRegsWriteReg(  tgRootRegs_t *regs, ushort addr, ulong value )
{
   switch( addr >> 2 )
   {
      case 0x02:

         regs->videoMuxMode = value & 0xffff;

         break;


      case 0x05:

         regs->gpoPort = value;

         break;

      case 0x06:

         //reset tick timer
         
         if( value & 1 )
         {
            regs->tickTimerValue = SDL_GetTicks();
         }

         break;

      case 0x08:

         //frame timer

         regs->frameTimer = 0;

         break;
   }


   return RV_OK;
}
