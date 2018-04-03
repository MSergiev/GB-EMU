#ifndef DISPLAY_H
#define DISPLAY_H

#include "TDraw.h"
#include "REGISTERS.h"

#define W 160
#define H 144

#define LCDC    0xFF40

#define BG1     0x9800
#define BG2     0x9C00
#define BG_LEN  1024

#define TLS1    0x8800
#define TLS2    0x8000
#define TLS_LEN 4096

static const BYTE PAL[] = { 0, 6, 3, 1 };

class DISPLAY {
    
private:
    
    TDraw tdraw;
    WORD BGLoc;
    WORD TSLoc;
    
    WORD cX, cY;
    
    BP CSL, SCRX, SCRY;
    
    bool HBLANK, VBLANK;
    
public:
    
    // Constructor
    DISPLAY() {
        CSL = &MEM[0xFF44];
        SCRX = &MEM[0xFF42];
        SCRY = &MEM[0xFF43];
    }
    
    // Destuctor
    ~DISPLAY(){
    }
    
    // Init
    inline void init() {
        tdraw.init();
    }    
    
    // Denit
    inline void deinit() {
        tdraw.deinit();
    }
    
    // Draw method
    inline void draw() {
        
        HBLANK = false;
        VBLANK = false;
        cX = 0; cY = 0;
        BGLoc = ( GB(&MEM[LCDC], 3) ? BG2 : BG1 );
        TSLoc = ( GB(&MEM[LCDC], 4) ? TLS1 : TLS2 );
        
        drawBG();
        tdraw.draw();
        
    }
    
private:
    
    // Draw background layer
    inline void drawBG() {
        
        for( BYTE i = 0; i < 0x29; ++i ) {
//             for( BYTE j = 0; j < 32; ++j ) {
                drawTile( i/*0x8280+i*/ );
//             }
        }
        
    }
    
    inline void drawTile( WORD idx ) {
        for( BYTE i = 0; i < 8; ++i ) {
            for( BYTE j = 0; j < 8; ++j ) {
                tdraw.drawPixel( cX+j, cY+i, PAL[pix(idx+i,j*2)] );
            }
        }
        updateCursor();
    }
    
    inline BYTE pix( BYTE loc, BYTE idx ) {
        return ( (GB(&MEM[TSLoc+loc+1],idx)<<1) | GB(&MEM[TSLoc+loc],idx) );
    }
    
    inline void updateCursor() {
        cX+=8;
        if(cX > W) {
            HBLANK = true;
            cX = 0;
            cY+=8;
            if( cY > H ) {
                VBLANK = true;
                cY = 0;
            }
        }
    }
};

#endif
