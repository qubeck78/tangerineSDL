
#include "memio.h"


static tangerineCtx_t *tgctx;

ulong mioInit( tangerineCtx_t *ptgctx )
{

   tgctx = ptgctx;

   return 0;
}

ulong fetchInstruction( ulong addr )
{

   return fetchData( addr );
}

ulong fetchData( ulong addr )
{
   ulong *loadPtr;

   //system ram ( stack, bootloader - if present, text mode )
   if( addr < 0x8000 )
   {
      return ( ( ulong* ) tgctx->systemRAM )[ addr >> 2 ];
   }

   //dma ram ( 8MB )
   else if( ( addr >= 0x20000000 ) && ( addr < 0x20800000 ) )
   {
      return ( ( ulong* ) tgctx->dmaRAM )[ (addr - 0x20000000 ) >> 2 ];

   }

   //fast ram ( 1MB )
   else if( ( addr >= 0x30000000 ) && ( addr < 0x30100000 ) )
   {
      return ( ( ulong* ) tgctx->fastRAM )[ (addr - 0x30000000 ) >> 2 ];

   }


   //registers
   else if( addr >= 0xf0000000 )
   {
      
      switch( addr & 0xffff0000 )
      {
         ulong rv;

         case 0xf0000000:

            //root regs

            return rootRegsReadReg( &tgctx->rootRegs, addr & 0xffff );

            break;

         case 0xf0800000:

            //sdram dma regs
            return sdramDMARegsReadReg( &tgctx->sdramDMARegs, addr & 0xffff );
            
            break;

      }
      return 0;
   }

   return 0;
}


ulong storeData( ulong addr, uchar mask, ulong data )
{
   ulong *storePtr;
   ulong rb;

   storePtr = NULL;


   //system ram ( stack, bootloader - if present, text mode )
   if( addr < 0x8000 )
   {
      storePtr = &( ( ulong* ) tgctx->systemRAM )[ addr >> 2 ];
   }

   //dma ram ( 8MB )
   else if( ( addr >= 0x20000000 ) && ( addr < 0x20800000 ) )
   {
      storePtr = &( ( ulong* ) tgctx->dmaRAM )[ (addr - 0x20000000 ) >> 2 ];
   }

   //fast ram ( 1MB )
   else if( ( addr >= 0x30000000 ) && ( addr < 0x30100000 ) )
   {
      storePtr = &( ( ulong* ) tgctx->fastRAM )[ (addr - 0x30000000 ) >> 2 ];
   }

   //registers
   else if( addr >= 0xf0000000 )
   {
      switch( addr & 0xffff0000 )
      {
         case 0xf0000000:

            //root regs

            return rootRegsWriteReg( &tgctx->rootRegs, addr & 0xffff, data );
            break;

         case 0xf0800000:

            //sdram dma regs
            
            return sdramDMARegsWriteReg( &tgctx->sdramDMARegs, addr & 0xffff, data );
            break;

      }
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


