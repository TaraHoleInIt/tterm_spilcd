#ifndef _F437_H_
#define _F437_H_

#if ! defined bit
    #define bit( n ) ( 1UL << n )
#endif

#include <stdint.h>

class F437 {
private:
    const uint8_t* _FontData;
    int _FontWidthBytes;
    int _FontWidth;
    int _FontHeight;

    int GetGlyphOffset( uint8_t c );

    void AddUnderline( uint16_t* Buffer, uint16_t Color );
    void AddStrike( uint16_t* Buffer, uint16_t Color );

    void Render0( uint8_t c, uint16_t* DestBuffer, bool Strike, bool Underline, uint16_t FGColor, uint16_t BGColor );
    void Render90( uint8_t c, uint16_t* DestBuffer, bool Strike, bool Underline, uint16_t FGColor, uint16_t BGColor );

    void ( F437::*RenderPtr ) ( uint8_t c, uint16_t* DestBuffer, bool Strike, bool Underline, uint16_t FGColor, uint16_t BGColor );
public:
    int GetFontWidth( void ) { return _FontWidth; }
    int GetFontHeight( void ) { return _FontHeight; }

    F437( const uint8_t* Fontdata, int Rotation );

    void Render( uint8_t c, uint16_t* DestBuffer, bool Strike, bool Underline, uint16_t FGColor, uint16_t BGColor );
};

extern const uint8_t Font_9x16[ ];
extern const uint8_t Font_8x16[ ];
extern const uint8_t Font_8x8[ ];
extern const uint8_t Font_6x8[ ];

#endif
