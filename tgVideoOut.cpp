#include "tgVideoOut.h"

#include <SDL2/SDL.h>

static uint32_t tgputChar( tangerineCtx_t *ctx, uint16_t *frameBuffer, uint16_t x, uint16_t y, uint8_t letter, uint8_t format )
{
    uint16_t   cx;
    uint16_t   cy; 
    uint16_t   cxMax;
    uint16_t   cxStep;

    uint8_t   *letterPtr;
    uint8_t    letterData;

    uint16_t   letterColor = 0xffff;
    uint16_t   backgroundColor = 0x0000;
    uint16_t   pixel;

    letterPtr = &ctx->consoleFont[ letter * 8 ];

    //letter color
    switch( format & 0x0f )
    {
        case 0:
            letterColor = tgColor565( 0x00, 0x00, 0x00 );
            break;

        case 1:
            letterColor = tgColor565( 0x80, 0x00, 0x00 );
            break;

        case 2:
            letterColor = tgColor565( 0x00, 0x80, 0x00 );
            break;

        case 3:
            letterColor = tgColor565( 0x00, 0x00, 0x80 );
            break;

        case 4:
            letterColor = tgColor565( 0x80, 0x80, 0x00 );
            break;

        case 5:
            letterColor = tgColor565( 0x00, 0x80, 0x80 );
            break;

        case 6:
            letterColor = tgColor565( 0x80, 0x00, 0x80 );
            break;

        case 7:
            letterColor = tgColor565( 0x40, 0x40, 0x40 );
            break;

        case 8:
            letterColor = tgColor565( 0x80, 0x80, 0x80 );
            break;

        case 9:
            letterColor = tgColor565( 0xff, 0x00, 0x00 );
            break;

        case 10:
            letterColor = tgColor565( 0x00, 0xff, 0x00 );
            break;

        case 11:
            letterColor = tgColor565( 0x00, 0x00, 0xff );
            break;

        case 12:
            letterColor = tgColor565( 0xff, 0xff, 0x00 );
            break;

        case 13:
            letterColor = tgColor565( 0x00, 0xff, 0xff );
            break;

        case 14:
            letterColor = tgColor565( 0xff, 0x00, 0xff );
            break;

        case 15:
            letterColor = tgColor565( 0xff, 0xff, 0xff );
            break;
    }

    //background color
    switch( ( format >> 4 ) & 0x0f )
    {
        case 0:
            backgroundColor = tgColor565( 0x00, 0x00, 0x00 );
            break;

        case 1:
            backgroundColor = tgColor565( 0x80, 0x00, 0x00 );
            break;

        case 2:
            backgroundColor = tgColor565( 0x00, 0x80, 0x00 );
            break;

        case 3:
            backgroundColor = tgColor565( 0x00, 0x00, 0x80 );
            break;

        case 4:
            backgroundColor = tgColor565( 0x80, 0x80, 0x00 );
            break;

        case 5:
            backgroundColor = tgColor565( 0x00, 0x80, 0x80 );
            break;

        case 6:
            backgroundColor = tgColor565( 0x80, 0x00, 0x80 );
            break;

        case 7:
            backgroundColor = tgColor565( 0x20, 0x20, 0x20 );
            break;

        case 8:
            backgroundColor = tgColor565( 0x80, 0x80, 0x80 );
            break;

        case 9:
            backgroundColor = tgColor565( 0xff, 0x00, 0x00 );
            break;

        case 10:
            backgroundColor = tgColor565( 0x00, 0xff, 0x00 );
            break;

        case 11:
            backgroundColor = tgColor565( 0x00, 0x00, 0xff );
            break;

        case 12:
            backgroundColor = tgColor565( 0xff, 0xff, 0x00 );
            break;

        case 13:
            backgroundColor = tgColor565( 0x00, 0xff, 0xff );
            break;

        case 14:
            backgroundColor = tgColor565( 0xff, 0x00, 0xff );
            break;

        case 15:
            backgroundColor = tgColor565( 0xff, 0xff, 0xff );
            break;
    }


    if( ctx->rootRegs.videoMuxMode & 0x04 )
    {
        //80 column mode

        y       *= 16;
        x       *= 8;
        cxMax    = 8;
        cxStep   = 1;

    }
    else
    {
        //40 column mode

        y       *= 16;
        x       *= 16;
        cxMax    = 16;
        cxStep   = 2;
    }



    for( cy = 0; cy < 16; cy += 2 )
    {
        letterData = *letterPtr++;

        for( cx = 0; cx < cxMax; cx += cxStep )
        {
            if( letterData & 0x80 )
            {
                frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x ]      = letterColor;
                frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x ]   = letterColor;

                if( cxStep == 2 )
                {
                    //40 columns
                    frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x + 1 ]      = letterColor;
                    frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x + 1 ]   = letterColor;
                }

            }
            else
            {
                if( ctx->rootRegs.videoMuxMode & 0x02 )
                {
                    //mix gfx with text
                    if( backgroundColor != 0x0000 )
                    {
                        if( backgroundColor != tgColor565( 0x80, 0x80, 0x80 ) )
                        {
                            //background color

                            frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x ]      = backgroundColor;
                            frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x ]   = backgroundColor;

                            if( cxStep == 2 )
                            {
                                //40 columns
                                frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x + 1 ]      = backgroundColor;
                                frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x + 1 ]   = backgroundColor;
                            }

                        }
                        else
                        {
                            //dim background

                            pixel = frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x ];
                            frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x ] = tgColor565( tgColor565GetR( pixel ) >> 1, 
                                tgColor565GetG( pixel ) >> 1, tgColor565GetB( pixel ) >> 1 );

                            pixel = frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x  ];
                            frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x  ] = tgColor565( tgColor565GetR( pixel ) >> 1, 
                                tgColor565GetG( pixel ) >> 1, tgColor565GetB( pixel ) >> 1 );
                            
                            if( cxStep == 2 )
                            {
                                pixel = frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x + 1 ];
                                frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x + 1 ] = tgColor565( tgColor565GetR( pixel ) >> 1, 
                                tgColor565GetG( pixel ) >> 1, tgColor565GetB( pixel ) >> 1 );

                                pixel = frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x + 1 ];
                                frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x + 1 ] = tgColor565( tgColor565GetR( pixel ) >> 1, 
                                tgColor565GetG( pixel ) >> 1, tgColor565GetB( pixel ) >> 1 );
                            }
                        }

                    }

                }
                else
                {
                    //text only

                    frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x ]      = backgroundColor;
                    frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x ]   = backgroundColor;

                    if( cxStep == 2 )
                    {
                        //40 columns
                        frameBuffer[ ( ( cy + y )  * 640 ) +  cx + x + 1 ]      = backgroundColor;
                        frameBuffer[ ( ( cy + y + 1 ) * 640 ) +  cx + x + 1 ]   = backgroundColor;
                    }

                }


            }

            letterData <<= 1;


        }


    }

    return 0;
}

uint32_t tgRedrawScreen( tangerineCtx_t *ctx )
{
    int          texturePitch;
    void        *texturePixels;
    
    uint16_t      *frameBuffer;
    uint16_t      *textureBuffer;
    uint16_t       pixel;

    uint8_t       *textOverlayPtr;
    uint32_t        toWidth;
    uint8_t        letter;
    uint8_t        format;


    uint32_t        x;
    uint32_t        y;

   SDL_LockTexture( ctx->texture, NULL, &texturePixels, &texturePitch );

   switch( ctx->rootRegs.videoMuxMode )
   {
      case _VIDEOMODE_640_TEXT40_OVER_GFX:
      case _VIDEOMODE_640_TEXT80_OVER_GFX:

          
          //memcpy( texturePixels, (const void*)bsp->dmaDisplayPointerStart, texturePitch * 480 );

         break;

      case _VIDEOMODE_320_TEXT40_OVER_GFX:
      case _VIDEOMODE_320_TEXT80_OVER_GFX:

         
         frameBuffer     = &( (uint16_t*) ctx->dmaRAM )[ ctx->sdramDMARegs.ch3DmaPointerStart << 1 ];

         textureBuffer   = (uint16_t*)texturePixels;


         for( y = 0; y < 240; y++ )
         {
            for( x = 0; x < 320; x++ )
            {
               pixel = *frameBuffer++;

               textureBuffer[ ( x * 2 ) + ( y * 2 ) * 640 ]                = pixel;
               textureBuffer[ ( x * 2 ) + 1 + ( y * 2 ) * 640 ]            = pixel;
               textureBuffer[ ( x * 2 ) + ( ( y * 2 ) + 1 ) * 640 ]        = pixel;
               textureBuffer[ ( x * 2 ) + 1 + ( ( y * 2 ) + 1 ) * 640 ]    = pixel;

            }

            if( y & 1 )
            {
               frameBuffer += ( ctx->sdramDMARegs.ch3DmaRequest1Modulo << 1 );
            }
            else
            {
               frameBuffer += ( ctx->sdramDMARegs.ch3DmaRequest0Modulo << 1 );
            }

         }

         break;
   }

   //text overlay

   if( ctx->rootRegs.videoMuxMode & 0x04 )
   {
      //80 column mode
      toWidth = 80;
   }
   else
   {
      //40 column mode
      toWidth = 40;
   }

   textOverlayPtr = &((uint8_t*)ctx->systemRAM)[0x6d40];
   
   for( y = 0; y < 30; y++ )
   {
      for( x = 0; x < toWidth; x++ )
      {
          letter = *textOverlayPtr++;
          format = *textOverlayPtr++;

          tgputChar( ctx, (uint16_t*)texturePixels, x, y, letter, format );
      }
   }  



   SDL_UnlockTexture( ctx->texture );

   SDL_RenderClear( ctx->renderer );
   SDL_RenderCopy( ctx->renderer, ctx->texture, NULL, NULL );
   SDL_RenderPresent( ctx->renderer );

   return RV_OK;
}

