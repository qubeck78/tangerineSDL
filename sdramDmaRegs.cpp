#include "sdramDmaRegs.h"

ulong sdramDMARegsInit( tgSDRAMDMARegs_t *regs )
{

   regs->id                   = 0x80000003;
   regs->version              = 0x20240820;

   regs->ch3DmaPointerStart   = 0;
   
   regs->ch3DmaRequest0Modulo = 96;
   regs->ch3DmaRequest1Modulo = 96;

   return RV_OK;
}

ulong sdramDMARegsReadReg(  tgSDRAMDMARegs_t *regs, ushort addr )
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

         return regs->ch3DmaPointerStart;

         break;

      case 0x03:

         return regs->ch3DmaRequest0Modulo;

         break;

      case 0x04:

         return regs->ch3DmaRequest1Modulo;

         break;
   }

   return 0;
}

ulong sdramDMARegsWriteReg(  tgSDRAMDMARegs_t *regs, ushort addr, ulong value )
{
   switch( addr >> 2 )
   {

      case 0x02:

         regs->ch3DmaPointerStart = value & 0xffffff;

         break;

      case 0x03:

         regs->ch3DmaRequest0Modulo = value & 0xff;

         break;

      case 0x04:

         regs->ch3DmaRequest1Modulo = value & 0xff;

         break;
   }

   return RV_OK;
}

