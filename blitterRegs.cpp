
#include "blitterRegs.h"
#include "memio.h"
#include "tangerine.h"

/*
uint32_t fetchData( uint32_t addr );
uint32_t storeData( uint32_t addr, uint8_t mask, uint32_t data );
*/

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

   //0x20800000

   memAddr = 0x20000000 | ( addr & 0x7ffffc ) ;


   return storeData( memAddr, memMask, memData );
}

static uint32_t bltRead( uint32_t addr )
{
   uint32_t memData;
   uint32_t memAddr;


   //0x20800000

   memAddr = 0x20000000 | ( addr & 0x7ffffc ) ;
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


   destAddress = regs->daAddress & 0x7ffffe;


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


   srcAddress  = regs->saAddress & 0x7ffffe;
   destAddress = regs->daAddress & 0x7ffffe;


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


   srcAAddress  = regs->saAddress & 0x7ffffe;
   srcBAddress  = regs->sbAddress & 0x7ffffe;
   destAddress = regs->daAddress & 0x7ffffe;


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

static uint32_t blitterCommand( tgBlitterRegs_t *regs, uint32_t cmd )
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

   }

   return RV_OK;
}


uint32_t blitterRegsInit( tgBlitterRegs_t *regs )
{
   regs->id             = 0x80000005;
   regs->version        = 0x20240916;
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

