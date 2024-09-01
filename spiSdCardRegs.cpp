#include "spiSDCardRegs.h"

ulong spiSdCardRegsInit( tgSpiSDCardRegs_t *regs )
{
   
   regs->id                   = 0x80000002;
   regs->version              = 0x20240820;
   regs->spiData              = 0x0;
   regs->spiStatus            = 0x1;   //spi ready


   return RV_OK;
}

ulong spiSdCardRegsReadReg(  tgSpiSDCardRegs_t *regs, ushort addr )
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

         return regs->spiData;

         break;

      case 0x03:

         return regs->spiStatus;

         break;
      }


   return 0;
}

ulong spiSdCardRegsWriteReg(  tgSpiSDCardRegs_t *regs, ushort addr, ulong value )
{


   return RV_OK;
}

