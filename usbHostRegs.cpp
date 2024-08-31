
#include "usbHostRegs.h"

ulong usbHostRegsInit( tgUsbHostRegs_t *regs )
{
   regs->id                   = 0x80000004;
   regs->version              = 0x20240820;
   regs->usbHidKeyboardData   = 0x0;
   regs->usbHidKeyboardStatus = 0x1;   //keyboard fifo empty

   return RV_OK;
}

ulong usbHostRegsReadReg(  tgUsbHostRegs_t *regs, ushort addr )
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

         return regs->usbHidKeyboardStatus;

         break;

      case 0x03:

         return regs->usbHidKeyboardData;

         break;
      }

   return 0;
}

ulong usbHostRegsWriteReg(  tgUsbHostRegs_t *regs, ushort addr, ulong value )
{

   return RV_OK;
}

