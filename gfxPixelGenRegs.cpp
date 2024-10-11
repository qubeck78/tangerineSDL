#include "gfxPixelGenRegs.h"

#include <cstdio>

uint32_t gfxPixelGenRegsInit( tgGfxPixelGenRegs_t *regs )
{
   uint32_t i;

   for( i = 0; i < 256; i++ )
   {
      regs->palette[i] = 0;
   }

   regs->id       = 0x80000008;
   regs->version  = 0x20241011;

   return 0;
}

uint32_t gfxPixelGenRegsReadReg( tgGfxPixelGenRegs_t *regs, uint32_t addr )
{

   if( addr & 0x20000 )
   {

      return regs->palette[ ( addr >> 2 ) & 0xff ];
   }
   else
   {

      switch( addr >> 2 )
      {
         case 0x00:

            return regs->id;

            break;

         case 0x01:

            return regs->version;

            break;

         default:

            return 0;
            break;

      }

   }
   return 0;
}

uint32_t gfxPixelGenRegsWriteReg(  tgGfxPixelGenRegs_t *regs, uint32_t addr, uint32_t value )
{
   if( addr & 0x20000 )
   {

      regs->palette[ ( addr >> 2 ) & 0xff ] = value & 0xffffff;

   }



   return 0;
}
