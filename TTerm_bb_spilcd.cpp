// Copyright (c) 2020 Tara Keeling
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include <Arduino.h>
#include <bb_spi_lcd.h>
#include <TTerm_bb_spilcd.h>

TTerm_SPILCD::TTerm_SPILCD( int DisplayWidth, int DisplayHeight, int FontWidth, int FontHeight ) : TTermbase( DisplayWidth, DisplayHeight, FontWidth, FontHeight ) {
    _FontWidth = FontWidth;
    _FontHeight = FontHeight;
}

size_t TTerm_SPILCD::DrawGlyph( int x, int y, uint8_t Character, uint16_t Attrib ) {
    uint16_t FG = 0;
    uint16_t BG = 0;
    uint16_t Temp = 0;

    FG = FGColorFromAttrib( Attrib );
    BG = BGColorFromAttrib( Attrib );

    if ( Attrib & Attrib_Reverse ) {
        Temp = FG;
        FG = BG;
        BG = Temp;
    }

    Character = Character == 0 ? ' ' : Character;
    uint8_t Buffer[ 2 ] = {
        Character, 0
    };

    if ( Character == 0 || ( Attrib & Attrib_Conceal ) ) {
        spilcdRectangle( x, y, _FontWidth, _FontHeight, 0x0000, 0x0000, 1, 1 );
    } else {
        if ( ! ( Attrib & Attrib_Conceal ) ) {
            spilcdWriteString( x, y, ( char* ) Buffer, FG, BG, FONT_SMALL, 1 );

            if ( Attrib & Attrib_Strike ) {
                spilcdDrawLine( x, y + ( _FontHeight / 2 ), x + _FontWidth - 2, y + ( _FontHeight / 2 ), FG, 1 );
            }

            if ( Attrib & Attrib_Underline ) {
                spilcdDrawLine( x, y + _FontHeight - 1, x + _FontWidth - 2, y + _FontHeight - 1, FG, 1 );
            }
        }
    }

    return 1;
}

uint16_t TTerm_SPILCD::FGColorFromAttrib( uint16_t Attrib ) {
    int r = 0;
    int g = 0;
    int b = 0;

    r = ( Attrib & Attrib_FR ) ? 15 : 0;
    g = ( Attrib & Attrib_FG ) ? 31 : 0;
    b = ( Attrib & Attrib_FB ) ? 15 : 0;

/*
    r |= ( Attrib & Attrib_I ) ? 0x08 : 0;
    g |= ( Attrib & Attrib_I ) ? 0x10 : 0;
    b |= ( Attrib & Attrib_I ) ? 0x08 : 0;
*/

    r <<= ( Attrib & Attrib_I ) ? 1 : 0;
    g <<= ( Attrib & Attrib_I ) ? 1 : 0;
    b <<= ( Attrib & Attrib_I ) ? 1 : 0;

    r |= ( Attrib & Attrib_I ) ? 1 : 0;
    g |= ( Attrib & Attrib_I ) ? 1 : 0;
    b |= ( Attrib & Attrib_I ) ? 1 : 0;

    return ( r << 11 ) | ( g << 5 ) | b;
}

uint16_t TTerm_SPILCD::BGColorFromAttrib( uint16_t Attrib ) {
    int r = 0;
    int g = 0;
    int b = 0;

    r = ( Attrib & Attrib_BR ) ? 31 : 0;
    g = ( Attrib & Attrib_BG ) ? 63 : 0;
    b = ( Attrib & Attrib_BB ) ? 31 : 0;

    return ( r << 11 ) | ( g << 5 ) | b;
}
