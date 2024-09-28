
#include "audioRegs.h"

uint32_t audioRegsInit( tgAudioRegs_t *regs )
{

   regs->id                = 0x80000007;
   regs->version           = 0x20240926;
   regs->audioFiFoData     = 0;
   regs->audioFiFoStatus   = 0;
   regs->i2sClockConfig    = 0x00410020;
   regs->fifoReadConfig    = 0;
   regs->audioDmaConfig    = 0;
   regs->audioDmaStatus    = 0;
   regs->audioDmaPointer   = 0;
   regs->audioDmaLength    = 0;
   

   return RV_OK;
}

uint32_t audioRegsReadReg(  tgAudioRegs_t *regs, audContext_t *audCtx, uint16_t addr )
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

         return regs->audioFiFoData;
         break;

      case 0x03:

         return regs->audioFiFoStatus;
         break;

      case 0x04:

         return regs->i2sClockConfig;
         break;

      case 0x05:

         return regs->fifoReadConfig;
         break;

      case 0x06:

         return regs->audioDmaConfig;
         break;

      case 0x07:

         return regs->audioDmaStatus;
         break;

      case 0x08:

         return regs->audioDmaPointer;
         break;

      case 0x09:

         return regs->audioDmaLength;
         break;

      default:
         
         return 0;
         break;

   }
   return 0;
}

uint32_t audioRegsWriteReg( tgAudioRegs_t *regs, audContext_t *audCtx, uint16_t addr, uint32_t value )
{
   switch( addr >> 2 )
   {

      case 0x02:

         //return regs->audioFiFoData;
         break;

      case 0x04:

         regs->i2sClockConfig = value;
         break;

      case 0x05:

         regs->fifoReadConfig = value;
         break;

      case 0x06:

         regs->audioDmaConfig = value;
         break;

      case 0x08:

         regs->audioDmaPointer = value;
         break;

      case 0x09:

         regs->audioDmaLength = value;
         break;
   }

   return RV_OK;
}
