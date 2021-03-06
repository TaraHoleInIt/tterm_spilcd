// Copyright (c) 2020 Tara Keeling
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include <Arduino.h>
#include <bb_spi_lcd.h>
#include <TTerm_bb_spilcd.h>
#include "f437.h"

void TTerm_SPILCD::Begin( int DisplayWidth, int DisplayHeight, F437* Font ) {
    _Font = Font;
    _FontWidth = Font->GetFontWidth( );
    _FontHeight = Font->GetFontHeight( );
    FGColor = 0;
    BGColor = 0;

    TTermbase::Begin( DisplayWidth, DisplayHeight, _FontWidth, _FontHeight );
}

void TTerm_SPILCD::DrawGlyph( int x, int y, char Character, uint16_t Attrib ) {
    uint16_t Buffer[ _FontWidth * _FontHeight ];
    uint16_t Temp = 0;

    FGColor = FGColorFromAttrib( Attrib );
    BGColor = BGColorFromAttrib( Attrib );

    _Font->Render( Character, Buffer, false, false, FGColor, BGColor );

    spilcdSetPosition( x, y, _FontWidth, _FontHeight, 1 );
    spilcdWriteDataBlock( ( uint8_t* ) Buffer, _FontWidth * _FontHeight * 2, 1 );

#if 0
    // bb_spilcd Doesn't render character 0, so instead we'll change it into a blank space
    Character = ( Character == 0 ) ? ' ' : Character;

    if ( Attrib & Attrib_Reverse ) {
        // Reverse foreground and background colours
        Temp = FGColor;
        FGColor = BGColor;
        BGColor = Temp;
    }

    if ( ! ( Attrib & Attrib_Conceal ) ) {
        // Character is visible
        DrawCharacter( x, y, Character );

        if ( Attrib & Attrib_Underline ) {
            DrawUnderline( x, y );
        }

        if ( Attrib & Attrib_Strike ) {
            DrawStrike( x, y );
        }
    } else {
        // Character is invisible
        DrawBlankCharacter( x, y );
    }
#endif
}

uint16_t TTerm_SPILCD::FGColorFromAttrib( uint16_t Attrib ) {
    int r = 0;
    int g = 0;
    int b = 0;

    // Start out with full intensity
    r = ( Attrib & Attrib_FR ) ? 31 : 0;
    g = ( Attrib & Attrib_FG ) ? 63 : 0;
    b = ( Attrib & Attrib_FB ) ? 31 : 0;

    // Halve intensity if the intensity attribute is not set
    r>>= ( ! ( Attrib & Attrib_I ) ) ? 1 : 0;
    g>>= ( ! ( Attrib & Attrib_I ) ) ? 1 : 0;
    b>>= ( ! ( Attrib & Attrib_I ) ) ? 1 : 0;

    return ( r << 11 ) | ( g << 5 ) | b;
}

uint16_t TTerm_SPILCD::BGColorFromAttrib( uint16_t Attrib ) {
    int r = 0;
    int g = 0;
    int b = 0;

    // For now, all background colours should be full intensity
    r = ( Attrib & Attrib_BR ) ? 31 : 0;
    g = ( Attrib & Attrib_BG ) ? 63 : 0;
    b = ( Attrib & Attrib_BB ) ? 31 : 0;

    return ( r << 11 ) | ( g << 5 ) | b;
}

void TTerm_SPILCD::Update( void ) {
    TTermbase::Update( );
}

size_t TTerm_SPILCD::write( uint8_t Data ) {
    return TTermbase::write( Data );
}

void TTerm_SPILCD::DrawBlankCharacter( int x, int y ) {
    DrawCharacter( x, y, ' ' );

    //spilcdRectangle( x, y, _FontWidth, _FontHeight, 0, 0, 1, 1 );
}

void TTerm_SPILCD::DrawCharacter( int x, int y, char Character ) {
    char Buffer[ 2 ] = {
        Character, 0
    };

    spilcdWriteString(
        x,
        y,
        Buffer,
        FGColor,
        BGColor,
        _FontSize,
        1
    );
}

void TTerm_SPILCD::DrawUnderline( int x, int y ) {
    // Draw a line under the character
    spilcdDrawLine( 
        x, 
        y + _FontHeight,
        x + _FontWidth,
        y + _FontHeight,
        FGColor,
        1
    );
}

void TTerm_SPILCD::DrawStrike( int x, int y ) {
    // Draw a line through the character
    spilcdDrawLine(
        x,
        y + ( _FontHeight / 2 ),
        x + _FontWidth,
        y + ( _FontHeight / 2 ),
        FGColor,
        1
    );
}
