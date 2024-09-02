#include "spiSDCardRegs.h"

#include <cstdio>

ulong spiSdCardRegsInit( tgSpiSDCardRegs_t *regs )
{
   
   regs->id                   = 0x80000002;
   regs->version              = 0x20240820;
   regs->spiData              = 0x0;
   regs->spiStatus            = 0x1;   //spi ready


   return RV_OK;
}

ulong spiSdCardRegsReadReg(  tgSpiSDCardRegs_t *regs, sdcContext_t *sdctx, ushort addr )
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

         return sdcSPIRead( sdctx );

         break;

      case 0x03:

         //printf( "spiStatus read: %x\n", regs->spiStatus );

         return regs->spiStatus;

         break;
      }


   return 0;
}

ulong spiSdCardRegsWriteReg(  tgSpiSDCardRegs_t *regs, sdcContext_t *sdctx, ushort addr, ulong value )
{

   switch( addr >> 2 )
   {

      case 0x02:

         sdcSPIWrite( sdctx, value );
         
         break;

      }

   return RV_OK;
}

