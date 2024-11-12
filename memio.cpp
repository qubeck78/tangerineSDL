
#include "memio.h"


static tangerineCtx_t *tgctx;

uint32_t mioInit( tangerineCtx_t *ptgctx )
{

   tgctx = ptgctx;

   return 0;
}

uint32_t fetchInstruction( uint32_t addr )
{

   return fetchData( addr );
}

uint32_t fetchData( uint32_t addr )
{
   uint32_t *loadPtr;

   //system ram ( stack, bootloader - if present, text mode )
   if( addr < 0x8000 )
   {
      return ( ( uint32_t* ) tgctx->systemRAM )[ addr >> 2 ];
   }

   //dma ram ( 64MB )                             
   else if( ( addr >= 0x20000000 ) && ( addr < 0x24000000 ) )
   {
      return ( ( uint32_t* ) tgctx->dmaRAM )[ (addr - 0x20000000 ) >> 2 ];

   }

   //fast ram ( 1MB )
   else if( ( addr >= 0x30000000 ) && ( addr < 0x30100000 ) )
   {
      return ( ( uint32_t* ) tgctx->fastRAM )[ (addr - 0x30000000 ) >> 2 ];

   }


   //registers
   else if( addr >= 0xf0000000 )
   {
      
      switch( addr & 0xfff00000 )
      {
         uint32_t rv;

         case 0xf0000000:

            //root regs

            return rootRegsReadReg( &tgctx->rootRegs, addr & 0xffff );

            break;

         case 0xf0200000:

            //blitter regs
            return blitterRegsReadReg( &tgctx->blitterRegs, addr & 0xffff );
            
            break;

         case 0xf0300000:

            //usb host regs
            return usbHostRegsReadReg( &tgctx->usbHostRegs, &tgctx->usbHostContext, addr & 0xffff );
            
            break;

         case 0xf0500000:

            //spi sd card regs
            return spiSdCardRegsReadReg( &tgctx->spiSdCardRegs, &tgctx->sdCardContext, addr & 0xffff );
            
            break;

         case 0xf0600000:

            //audio ( i2s ) regs
            return audioRegsReadReg( &tgctx->audioRegs, &tgctx->audioContext, addr & 0xffff );
            
            break;

         case 0xf0800000:

            //sdram dma regs
            return sdramDMARegsReadReg( &tgctx->sdramDMARegs, addr & 0xffff );
            
            break;

         case 0xf0900000:

            //gfx pixel gen regs
            return gfxPixelGenRegsReadReg( &tgctx->gfxPixelGenRegs, addr & 0xfffff );
            
            break;

         default:
            
            //printf( "Warning: memio - unknown register read:%x\n", addr );
            break;
      }
      return 0;
   }
   else
   {
      printf( "Warning: memio - unknown read:%x\n", addr );
   }
   return 0;
}


uint32_t storeData( uint32_t addr, uint8_t mask, uint32_t data )
{
   uint32_t *storePtr;
   uint32_t rb;

   storePtr = NULL;


   //system ram ( stack, bootloader - if present, text mode )
   if( addr < 0x8000 )
   {
      storePtr = &( ( uint32_t* ) tgctx->systemRAM )[ addr >> 2 ];
   }


   //dma ram ( 64MB )
   else if( ( addr >= 0x20000000 ) && ( addr < 0x24000000 ) )
   {
      storePtr = &( ( uint32_t* ) tgctx->dmaRAM )[ (addr - 0x20000000 ) >> 2 ];
   }

   //fast ram ( 1MB )
   else if( ( addr >= 0x30000000 ) && ( addr < 0x30100000 ) )
   {
      storePtr = &( ( uint32_t* ) tgctx->fastRAM )[ (addr - 0x30000000 ) >> 2 ];
   }

   //registers
   else if( addr >= 0xf0000000 )
   {
      switch( addr & 0xfff00000 )
      {
         case 0xf0000000:

            //root regs

            return rootRegsWriteReg( &tgctx->rootRegs, &tgctx->sdCardContext, addr & 0xffff, data );
            break;

         case 0xf0200000:

            //usb host regs
            return blitterRegsWriteReg( &tgctx->blitterRegs, addr & 0xffff, data );
            
            break;

         case 0xf0300000:

            //usb host regs
            return usbHostRegsWriteReg( &tgctx->usbHostRegs, &tgctx->usbHostContext, addr & 0xffff, data );
            
            break;

         case 0xf0500000:

            //spi sd card regs
            return spiSdCardRegsWriteReg( &tgctx->spiSdCardRegs, &tgctx->sdCardContext, addr & 0xffff, data );
            
            break;

         case 0xf0600000:

            //audio ( i2s ) regs
            return audioRegsWriteReg( &tgctx->audioRegs, &tgctx->audioContext, addr & 0xffff, data );
            
            break;

         case 0xf0800000:

            //sdram dma regs
            
            return sdramDMARegsWriteReg( &tgctx->sdramDMARegs, addr & 0xffff, data );
            break;

         case 0xf0900000:

            //gfx pixel gen regs
            
            return gfxPixelGenRegsWriteReg( &tgctx->gfxPixelGenRegs, addr & 0xfffff, data );
            break;

         default:
            
            //printf( "Warning: memio - unknown register write:%x, %x\n", addr, data );
            break;
      }
   }   
   else
   {
      printf( "Warning: memio - unknown write:%x, %x\n", addr, data );
   }

   if( storePtr )
   {
      rb = *storePtr;

      if( mask & 1)
      {
         //0-7
         rb &= 0xffffff00;
         rb |= data & 0xff;
      }
      
      if( mask & 2)
      {
         //8-15
         rb &= 0xffff00ff;
         rb |= data & 0xff00;
      }

      if( mask & 4)
      {
         //16-23
         rb &= 0xff00ffff;
         rb |= data & 0xff0000;
      }

      if( mask & 8)
      {
         //24-32
         rb &= 0x00ffffff;
         rb |= data & 0xff000000;
      }

      *storePtr = rb;
   }

   return 0;
   
}


