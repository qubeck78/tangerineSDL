
#include "console.h"

#include <stdarg.h>

#define _IS_UPPER(c)    (((c)>='A')&&((c)<='Z'))
#define _IS_LOWER(c)    (((c)>='a')&&((c)<='z'))
#define _IS_DIGIT(c)    (((c)>='0')&&((c)<='9'))


uint32_t conInit( tangerineCtx_t *tgCtx, tdbgConsole *con )
{

   con->width           = 80;
   con->height          = 30;
   con->cursX           = 0;
   con->cursY           = 0;
   con->textAttributes  = 0x0f;
   
   con->textBuffer      = &(( uint8_t * )tgCtx->systemRAM)[0x6d40]; //2nd half of screen 80x60 text mode

   return 0;
}

uint32_t conSetCursorPos( tdbgConsole *con, uint16_t cursX, uint16_t cursY )
{
   
   if( !con )
   {
      return 1;
   }

   if( ( cursX < con->width ) && ( cursY < con->height ) )
   {

      con->cursX = cursX;
      con->cursY = cursY;

   }
   else
   {
      return 2;
   }

   return 0;
}

uint32_t conCls( tdbgConsole *con )
{

   uint32_t   i;

   if( !con )
   {
      return 1;
   }


   for( i = 0; i < ( con->width * con->height * 2 ); i += 2 )
   {
      con->textBuffer[i+1]      = con->textAttributes;
      con->textBuffer[i]        = 0;
   }

   con->cursX = 0;
   con->cursY = 0;

   return 0;
}

uint32_t conPrint( tdbgConsole *con, char *string )
{
   uint8_t  c;
   uint32_t idx;

   if( !con )
   {
      return 1;
   }

   while( ( c = *string++ ) )
   {
      idx = ( con->cursX * 2 ) + ( con->cursY * con->width * 2 );


      if( ( c > 0 ) && ( c != 10 ) )
      {

         con->textBuffer[idx++] = c;
         con->textBuffer[idx++] = con->textAttributes;

         con->cursX++;

         if( con->cursX >= con->width )
         {
            con->cursX = 0;
            con->cursY++;
         }
      }
      else if( c == 10 )
      {
         con->cursX = 0;
         con->cursY++;
      }


      if( con->cursY >= con->height )
      {
         con->cursY = 0;
      }
   }

   return 0;
}

uint32_t conPrintF( tdbgConsole *con, char *format, ... )
{
   va_list       arp;
   uint8_t       flag;
   uint32_t      radix;
   uint32_t      width;
   uint32_t      value;
   uint8_t       scratch[16];
   uint8_t      *pointer;

   uint8_t       c;
   uint8_t       d; 
   uint32_t      i; 
   uint32_t      j;

   char printBuf[2];

   printBuf[1] = 0;

   va_start( arp, format );

   for( ; ; ) 
   {

     c = *format++;

     if(c == 0) break;           /* End of string */

     if(c != '%') 
     {               

         /* Non escape character */
      printBuf[0] = c;
      conPrint( con, printBuf );

      continue;
   }


   width = 0;
   flag = 0;
   c = *format++;

   if( c == '0' ) 
   {               
         /* Flag: '0' padding */

      flag = 1; 
      c = *format++;

   }else
   {

      if( c == '-' ) 
      {           

             /* Flag: left justified */
       flag = 2; 
       c = *format++;
    }
 }

 while( _IS_DIGIT( c ) ) 
 {       

         /* Precision */
   width = width * 10 + c - '0';
   c = *format++;
}

if( (c == 'l') || (c == 'L') ) 
{   
         /* Prefix: Size is int32_t int */

   flag |= 4; 
   c = *format++;
}

if( !c ) break;

d = c;

if( _IS_LOWER( d ) ) d -= 0x20;

switch( d ) 
{               

         /* Type is... */

case 'S' :                  

             /* String */

   pointer = va_arg( arp, unsigned char* );

   for(j = 0; pointer[j] ; j++);

    if( !( flag & 2 ) ) 
    {
     while( j++ < width ) 
     {
      printBuf[0] = ' ';
      conPrint( con, printBuf );


   }
}

while( *pointer ) 
{
   printBuf[0] = *pointer++;
   conPrint( con, printBuf );

}
while( j++ < width ) 
{
   printBuf[0] = ' ';
   conPrint( con, printBuf );
}
continue;

         case 'C' :                  /* Character */

printBuf[0] = ( char ) va_arg( arp, int );
conPrint( con, printBuf );

continue;

         case 'B' :                  /* Binary */

radix = 2; 
break;

         case 'O' :                  /* Octal */
radix = 8; 
break;

         case 'D' :                  /* Signed decimal */
         case 'U' :                  /* Unsigned decimal */

radix = 10; 
break;

         case 'X' :                  /* Hexdecimal */

radix = 16; 
break;

         default:                    /* Unknown type (pass-through) */

printBuf[0] = c;
conPrint( con, printBuf );

continue;
}

     /* Get an argument and put it in numeral */

value = (flag & 4) ? (uint32_t) va_arg( arp, int32_t ) : ( ( d == 'D' ) ? (uint32_t)(int32_t) va_arg( arp, int ) : (uint32_t) va_arg( arp, unsigned int ) );


if( d == 'D' && ( value & 0x80000000) ) 
{
   value = 0 - value;
   flag |= 8;
}

i = 0;

do 
{

   d = (char)( value % radix ); 
   value /= radix;


   if ( d > 9 ) d += ( c == 'x' ) ? 0x27 : 0x07;

   scratch[i++] = d + '0';

}while( value && i < sizeof scratch / sizeof scratch[0] );

if( flag & 8 ) scratch[i++] = '-';

j = i; 
d = ( flag & 1 ) ? '0' : ' ';

while( ! ( flag & 2 ) && j++ < width )
{
   printBuf[0] = d;
   conPrint( con, printBuf );

}

do
{

   printBuf[0] = scratch[--i];
   conPrint( con, printBuf );

}while ( i );

while( j++ < width ) 
{
   printBuf[0] = d;
   conPrint( con, printBuf );
}
}

va_end( arp );

return 0;   
}
