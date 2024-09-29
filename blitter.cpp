#include "blitter.h"

#include "memio.h"
#include "tangerine.h"

static uint32_t bltWrite( uint32_t addr, uint16_t data )
{
   uint8_t  memMask;
   uint32_t memData;
   uint32_t memAddr;

   memData = ( data << 16 ) | data;

   if( addr & 2 )
   {
      memMask = 0b1100;
   }
   else
   {
      memMask = 0b0011;
   }

   //0x24000000
   memAddr = 0x20000000 | ( addr & 0x3fffffc ) ;


   return storeData( memAddr, memMask, memData );
}

static uint32_t bltRead( uint32_t addr )
{
   uint32_t memData;
   uint32_t memAddr;


   //0x20800000

   memAddr = 0x20000000 | ( addr & 0x3fffffc ) ;
   memData = fetchData( memAddr );

   if( addr & 2 )
   {
      return ( memData >> 16 ) & 0xffff;
   }
   else
   {
      return memData & 0xffff;
   }

}

static uint32_t bltCalcAlpha( uint32_t pixelA, uint32_t pixelB, uint32_t alpha )
{
   uint32_t ra;
   uint32_t ga;
   uint32_t ba;
   
   uint32_t rb;
   uint32_t gb;
   uint32_t bb;

   uint32_t rd;
   uint32_t gd;
   uint32_t bd;

   alpha = alpha * 8;

   ra = tgColor565GetR( pixelA );
   ga = tgColor565GetG( pixelA );
   ba = tgColor565GetB( pixelA );

   rb = tgColor565GetR( pixelB );
   gb = tgColor565GetG( pixelB );
   bb = tgColor565GetB( pixelB );
   
   rd = rb * ( 255 - alpha );
   gd = gb * ( 255 - alpha );
   bd = bb * ( 255 - alpha );
   
   rd += ra * alpha;
   gd += ga * alpha;
   bd += ba * alpha;
    
   rd >>= 8;
   gd >>= 8;
   bd >>= 8;
   
   return tgColor565( rd, gd, bd );
}

static uint32_t blitterFill( tgBlitterRegs_t *regs )
{
   
   uint32_t destAddress;
   uint32_t x;
   uint32_t y;


   destAddress = regs->daAddress & 0x3fffffc;


   for( y = 0; y < regs->daHeight; y++ )
   {
      for( x = 0; x < regs->daWidth; x++ )
      {

         bltWrite( destAddress, regs->input0 );
         destAddress += 2;
      }
      destAddress += regs->daColAdd * 2;
   }

   return RV_OK;
}

static uint32_t blitterCopy( tgBlitterRegs_t *regs, uint32_t cmd )
{
   
   uint32_t srcAddress;
   uint32_t destAddress;
   uint32_t x;
   uint32_t y;
   uint16_t pixel;


   srcAddress  = regs->saAddress & 0x3fffffc;
   destAddress = regs->daAddress & 0x3fffffc;


   if( cmd & 1 )
   {

      for( y = 0; y < regs->daHeight; y++ )
      {
         for( x = 0; x < regs->daWidth; x++ )
         {

            pixel = bltRead( srcAddress );

            if( pixel != ( regs->input0 & 0xffff ) )
            {
               bltWrite( destAddress, pixel );
            }

            srcAddress  += 2;
            destAddress += 2;

         }

         srcAddress  += regs->saColAdd * 2;
         destAddress += regs->daColAdd * 2;
      }

   }
   else
   {
      for( y = 0; y < regs->daHeight; y++ )
      {
         for( x = 0; x < regs->daWidth; x++ )
         {

            bltWrite( destAddress, bltRead( srcAddress ) );

            srcAddress  += 2;
            destAddress += 2;

         }

         srcAddress  += regs->saColAdd * 2;
         destAddress += regs->daColAdd * 2;
      }

   }

   return RV_OK;
}

static uint32_t blitterCopyAlpha( tgBlitterRegs_t *regs, uint32_t cmd )
{
   
   uint32_t srcAAddress;
   uint32_t srcBAddress;
   uint32_t destAddress;
   uint32_t x;
   uint32_t y;
   uint16_t pixelA;
   uint16_t pixelB;


   srcAAddress  = regs->saAddress & 0x3fffffc;
   srcBAddress  = regs->sbAddress & 0x3fffffc;
   destAddress = regs->daAddress & 0x3fffffc;


   if( cmd & 1 )
   {

      for( y = 0; y < regs->daHeight; y++ )
      {
         for( x = 0; x < regs->daWidth; x++ )
         {

            pixelA   = bltRead( srcAAddress );
            pixelB   = bltRead( srcBAddress );

            if( pixelA != ( regs->input1 & 0xffff ) )
            {
               bltWrite( destAddress, bltCalcAlpha( pixelA, pixelB, regs->input0 ) );
            }

            srcAAddress += 2;
            srcBAddress += 2;
            destAddress += 2;

         }

         srcAAddress += regs->saColAdd * 2;
         srcBAddress += regs->sbColAdd * 2;
         destAddress += regs->daColAdd * 2;
      }

   }
   else
   {
      for( y = 0; y < regs->daHeight; y++ )
      {
         for( x = 0; x < regs->daWidth; x++ )
         {


            pixelA   = bltRead( srcAAddress );
            pixelB   = bltRead( srcBAddress );

            bltWrite( destAddress, bltCalcAlpha( pixelA, pixelB, regs->input0 ) );

            srcAAddress += 2;
            srcBAddress += 2;
            destAddress += 2;

         }

         srcAAddress += regs->saColAdd * 2;
         srcBAddress += regs->sbColAdd * 2;
         destAddress += regs->daColAdd * 2;
      }

   }

   return RV_OK;
}

static uint32_t blitterScaledCopy( tgBlitterRegs_t *regs, uint32_t cmd )
{
   
   uint32_t srcAddress;
   uint32_t destAddress;
   uint32_t x;
   uint32_t y;
   uint16_t pixel;

   uint32_t sx;
   uint32_t sy;

   destAddress = regs->daAddress & 0x3fffffc;


   sx = 0;
   sy = 0;

   if( cmd & 1 )
   {

      for( y = 0; y < regs->daHeight; y++ )
      {


         for( x = 0; x < regs->daWidth; x++ )
         {

            srcAddress  = regs->saAddress & 0x3fffffc;
            srcAddress  += ( sx >> 15 ) + ( sy >> 16 ) * regs->saRowWidth * 2;
            sx += regs->input0;

            pixel = bltRead( srcAddress );

            if( pixel != ( regs->input2 & 0xffff ) )
            {
               bltWrite( destAddress, pixel );
            }

            srcAddress  += 2;
            destAddress += 2;

         }

         sx = 0;
         sy += regs->input1;

         destAddress += regs->daColAdd * 2;
      }

   }
   else
   {

      for( y = 0; y < regs->daHeight; y++ )
      {


         for( x = 0; x < regs->daWidth; x++ )
         {

            srcAddress  = regs->saAddress & 0x3fffffc;
            srcAddress  += ( sx >> 15 ) + ( sy >> 16 ) * regs->saRowWidth * 2;
            sx += regs->input0;

            bltWrite( destAddress, bltRead( srcAddress ) );

            srcAddress  += 2;
            destAddress += 2;

         }

         sx = 0;
         sy += regs->input1;

         destAddress += regs->daColAdd * 2;
      }

   }

   return RV_OK;
}


uint32_t blitterCommand( tgBlitterRegs_t *regs, uint32_t cmd )
{

   regs->saColAdd = regs->saRowWidth - regs->saWidth;
   regs->sbColAdd = regs->sbRowWidth - regs->saWidth;
   regs->scColAdd = regs->scRowWidth - regs->saWidth;
   
   regs->daColAdd = regs->daRowWidth - regs->daWidth;
   regs->dbColAdd = regs->dbRowWidth - regs->daWidth;


   switch( ( cmd >> 8 ) & 0xff )
   {

      case 0x01:

         return blitterFill( regs );
         break;

      case 0x02:

         return blitterCopy( regs, cmd );
         break;

      case 0x03:

         return blitterCopyAlpha( regs, cmd );
         break;

      case 0x04:
         return blitterScaledCopy( regs, cmd );

   }

   return RV_OK;
}

