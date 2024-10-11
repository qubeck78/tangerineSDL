#ifndef _CONSOLE_H
#define _CONSOLE_H

#include "gfTypes.h"
#include "tangerine.h"

typedef struct _tdbgConsole
{
   uint16_t        width;
   uint16_t        height;

   uint16_t        cursX;
   uint16_t        cursY;
   uint8_t         textAttributes;

   uint8_t        *textBuffer;
   
}tdbgConsole;



uint32_t conInit( tangerineCtx_t *tgCtx, tdbgConsole *con );

uint32_t conSetCursorPos( tdbgConsole *con, uint16_t cursX, uint16_t cursY );
uint32_t conCls( tdbgConsole *con );
uint32_t conScrollUp( tdbgConsole *con );
uint32_t conPrint( tdbgConsole *con, char *string );
uint32_t conPrintF( tdbgConsole *con, char *format, ... );

#endif
