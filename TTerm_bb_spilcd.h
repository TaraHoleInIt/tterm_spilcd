#ifndef _TTERM_BB_SPILCD_H_
#define _TTERM_BB_SPILCD_H_

#include <TTermBase.h>

class TTerm_SPILCD : public TTermbase, public Print {
private:
    size_t DrawGlyph( int x, int y, uint8_t Character, uint16_t Attrib );

    uint16_t FGColorFromAttrib( uint16_t Attrib );
    uint16_t BGColorFromAttrib( uint16_t Attrib );

    int _FontWidth;
    int _FontHeight;
public:
    TTerm_SPILCD( int DisplayWidth, int DisplayHeight, int FontWidth, int FontHeight );

    size_t write( uint8_t Data ) {
        return TTermbase::write( Data );
    }
};


#endif
