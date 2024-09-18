
#include "blitterRegs.h"
#include "blitter.h"


uint32_t blitterRegsInit( tgBlitterRegs_t *regs )
{
   regs->id             = 0x80000005;
   regs->version        = 0x20240918;
   regs->command        = 0;

   return RV_OK;
}

uint32_t blitterRegsReadReg( tgBlitterRegs_t *regs, uint16_t addr )
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

         //status, emulated blitter always ready
         return 1;
         break;

      case 0x03:

         return regs->input0;

         break;

      case 0x04:

         return regs->input1;

         break;

      case 0x05:

         return regs->input2;

         break;

      case 0x06:

         return regs->input3;

         break;

      case 0x07:

         return regs->saAddress;

         break;

      case 0x08:

         return regs->saRowWidth;

         break;

      case 0x09:

         return regs->saWidth;

         break;

      case 0x0a:

         return regs->saHeight;

         break;

      case 0x0b:

         return regs->sbAddress;

         break;

      case 0x0c:

         return regs->sbRowWidth;

         break;

      case 0x0d:

         return regs->scAddress;

         break;

      case 0x0e:

         return regs->scRowWidth;

         break;

      case 0x0f:

         return regs->daAddress;

         break;

      case 0x10:

         return regs->daRowWidth;

         break;

      case 0x11:

         return regs->daWidth;

         break;

      case 0x12:

         return regs->daHeight;

         break;

      case 0x13:

         return regs->dbAddress;

         break;

      case 0x14:

         return regs->dbRowWidth;

         break;

   }

   return 0;
}

uint32_t blitterRegsWriteReg(  tgBlitterRegs_t *regs, uint16_t addr, uint32_t value )
{

   switch( addr >> 2 )
   {

      case 0x02:

         //blitter command
         blitterCommand( regs, value );

         break;

      case 0x03:

         regs->input0 = value;

         break;

      case 0x04:

         regs->input1 = value;

         break;

      case 0x05:

         regs->input2 = value;

         break;

      case 0x06:

         regs->input3 = value;

         break;

      case 0x07:

         regs->saAddress = value;

         break;

      case 0x08:

         regs->saRowWidth = value;

         break;

      case 0x09:

         regs->saWidth = value;

         break;

      case 0x0a:

         regs->saHeight = value;

         break;

      case 0x0b:

         regs->sbAddress = value;

         break;

      case 0x0c:

         regs->sbRowWidth = value;

         break;

      case 0x0d:

         regs->scAddress = value;

         break;

      case 0x0e:

         regs->scRowWidth = value;

         break;

      case 0x0f:

         regs->daAddress = value;

         break;

      case 0x10:

         regs->daRowWidth = value;

         break;

      case 0x11:

         regs->daWidth = value;

         break;

      case 0x12:

         regs->daHeight = value;

         break;

      case 0x13:

         regs->dbAddress = value;

         break;

      case 0x14:

         regs->dbRowWidth = value;

         break;


   }

   return RV_OK;
}

