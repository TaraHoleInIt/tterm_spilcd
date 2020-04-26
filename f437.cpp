#include "f437.h"

F437::F437( const uint8_t* FontData, int Rotation ) {
    _FontData = &FontData[ 2 ];

    switch ( Rotation ) {
        case 0: 
        default: {
            // No rotation
            _FontWidth = FontData[ 0 ];
            _FontHeight = FontData[ 1 ];

            RenderPtr = &F437::Render0;
            break;
        }
    };

    _FontWidthBytes = ( _FontWidth + 7 ) / 8;
}

void F437::Render( uint8_t c, uint16_t* DestBuffer, bool Strike, bool Underline, uint16_t FGColor, uint16_t BGColor ) {
    ( this->*RenderPtr ) ( c, DestBuffer, Strike, Underline, FGColor, BGColor );
}

void F437::Render0( uint8_t c, uint16_t* DestBuffer, bool Strike, bool Underline, uint16_t FGColor, uint16_t BGColor ) {
    const uint8_t* Glyph = &_FontData[ GetGlyphOffset( c ) ];
    int Remainder = ( _FontWidth & 0x07 ) % 8;
    uint16_t* OutPtr = DestBuffer;
    int UnderlineHeight = 0;
    int StrikeHeight = 0;
    int x = 0;
    int y = 0;

    UnderlineHeight = ( Underline ) ? _FontHeight - 1 : -1;
    StrikeHeight = ( Strike ) ? ( _FontHeight / 2 ) - 1 : -1;

    for ( y = 0; y < _FontHeight; y++ ) {
        for ( x = 0; x < ( _FontWidth / 8 ); x++ ) {
            *OutPtr++ = ( y == StrikeHeight || y == UnderlineHeight ) ? FGColor : *Glyph & 0x80 ? FGColor : BGColor;
            *OutPtr++ = ( y == StrikeHeight || y == UnderlineHeight ) ? FGColor : *Glyph & 0x40 ? FGColor : BGColor;
            *OutPtr++ = ( y == StrikeHeight || y == UnderlineHeight ) ? FGColor : *Glyph & 0x20 ? FGColor : BGColor;
            *OutPtr++ = ( y == StrikeHeight || y == UnderlineHeight ) ? FGColor : *Glyph & 0x10 ? FGColor : BGColor;
            *OutPtr++ = ( y == StrikeHeight || y == UnderlineHeight ) ? FGColor : *Glyph & 0x08 ? FGColor : BGColor;
            *OutPtr++ = ( y == StrikeHeight || y == UnderlineHeight ) ? FGColor : *Glyph & 0x04 ? FGColor : BGColor;
            *OutPtr++ = ( y == StrikeHeight || y == UnderlineHeight ) ? FGColor : *Glyph & 0x02 ? FGColor : BGColor;
            *OutPtr++ = ( y == StrikeHeight || y == UnderlineHeight ) ? FGColor : *Glyph & 0x01 ? FGColor : BGColor;

            Glyph++;
        }

        if ( Remainder ) {
            for ( x = 7; x >= ( 8 - Remainder ); x-- ) {
               *OutPtr++ = ( y == StrikeHeight || y == UnderlineHeight ) ? FGColor : *Glyph & bit( x ) ? FGColor : BGColor;
            }

            Glyph++;
        }
    }
}

void F437::Render90( uint8_t c, uint16_t* DestBuffer, bool Strike, bool Underline, uint16_t FGColor, uint16_t BGColor ) {
}

int F437::GetGlyphOffset( uint8_t c ) {
    return c * _FontHeight * _FontWidthBytes;
}
