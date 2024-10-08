
#include "usbHostRegs.h"

uint32_t usbHostRegsInit( tgUsbHostRegs_t *regs )
{
   regs->id                   = 0x80000004;
   regs->version              = 0x20240820;
   regs->usbHidKeyboardData   = 0x0;
   regs->usbHidKeyboardStatus = 0x1;   //keyboard fifo empty

   return RV_OK;
}

uint32_t usbHostRegsReadReg( tgUsbHostRegs_t *regs, usbhContext_t *ctx, uint16_t addr )
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

         return usbHostFifoEmpty( ctx ) & 1;

         break;

      case 0x03:

         return usbHostFifoGet( ctx );

         break;
      }

   return 0;
}

uint32_t usbHostRegsWriteReg(  tgUsbHostRegs_t *regs, usbhContext_t *ctx, uint16_t addr, uint32_t value )
{

   return RV_OK;
}

