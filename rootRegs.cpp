#include "rootRegs.h"

#include <SDL2/SDL.h>

uint32_t rootRegsInit( tgRootRegs_t *regs )
{

   regs->id             = 0x80000000;
   regs->version        = 0x20241110;

   regs->videoMuxMode   = 0x0;
   regs->videoVSync     = 1;
   
   regs->tickTimerValue = SDL_GetTicks();
   regs->frameTimer     = 0;

   regs->mtime          = 0;
   regs->mtimeCmp       = 0;

   return RV_OK;

}

uint32_t rootRegsReadReg(  tgRootRegs_t *regs, uint16_t addr )
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

         if( regs->videoVSync )
         {
            regs->videoVSync = 0;
            return 1; 
         }
         else
         {
            return 0;
         }

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

         return (uint32_t)( SDL_GetTicks() - regs->tickTimerValue );
         
         break;

      case 0x08:

         //frame timer

         return regs->frameTimer;
         
         break;

      case 0x0a:

         //mtime lo
         return (uint32_t)( regs->mtime & 0xffffffff );

      case 0x0b:

         //mtime hi
         return (uint32_t)( ( regs->mtime >> 32 ) & 0xffffffff );

      case 0x0c:

         //mtimeCmp lo
         return (uint32_t)( regs->mtimeCmp & 0xffffffff );

      case 0x0d:

         //mtimeCmp hi
         return (uint32_t)( ( regs->mtimeCmp >> 32 ) & 0xffffffff );


   }

   return 0;
}

uint32_t rootRegsWriteReg(  tgRootRegs_t *regs, sdcContext_t *sdctx, uint16_t addr, uint32_t value )
{
   switch( addr >> 2 )
   {
      case 0x02:

         regs->videoMuxMode = value & 0xffff;

         break;


      case 0x05:

         regs->gpoPort = value;
         sdcSPICE( sdctx, value & 1 ); //sd CE connected to GPIO 0

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

      case 0x0a:

         //mtime lo
         //return (uint32_t)( regs->mtime & 0xffffffff );
         break;

      case 0x0b:

         //mtime hi
         //return (uint32_t)( ( regs->mtime >> 32 ) & 0xffffffff );
         break;

      case 0x0c:

         //mtimeCmp lo
         regs->mtimeCmp &= 0xffffffff00000000;
         regs->mtimeCmp |= (uint64_t)value;

         break;

      case 0x0d:

         //mtimeCmp hi
         regs->mtimeCmp &= 0x00000000ffffffff;
         regs->mtimeCmp |= ( (uint64_t) value ) << 32;

         break;
   }


   return RV_OK;
}
