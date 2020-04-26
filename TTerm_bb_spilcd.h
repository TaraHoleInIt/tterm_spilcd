#ifndef _TTERM_BB_SPILCD_H_
#define _TTERM_BB_SPILCD_H_

#include <TTermBase.h>
#include <f437.h>

class TTerm_SPILCD : public TTermbase, public Print {
private:
    void DrawGlyph( int x, int y, char Character, uint16_t Attrib );

    uint16_t FGColorFromAttrib( uint16_t Attrib );
    uint16_t BGColorFromAttrib( uint16_t Attrib );

    void DrawCharacter( int x, int y, char Character );
    void DrawBlankCharacter( int x, int y );
    void DrawUnderline( int x, int y );
    void DrawStrike( int x, int y );

    int _FontSize;
    int _FontWidth;
    int _FontHeight;

    uint16_t FGColor;
    uint16_t BGColor;

    F437* _Font;
public:
    void Begin( int DisplayWidht, int DisplayHeight, F437* Font );

    void Update( void );
    size_t write( uint8_t Data );
};


#endif
