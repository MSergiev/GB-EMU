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

#define TLS1    0x8000
#define TLS2    0x8800
#define TLS_LEN 4096

static const BYTE PAL[] = { 0, 6, 3, 8 };

class DISPLAY {
    
private:
    
    TDraw tdraw;
    WORD BGLoc;
    WORD TSLoc;
    
    WORD cX, cY;
    
    BP CSL, SCX, SCY;
    
    BYTE BG[65536];
    unsigned CURSOR;
    
    bool HBLANK, VBLANK;
    
public:
    
    // Constructor
    DISPLAY() {
        CSL = &MEM[0xFF44];
        SCX = &MEM[0xFF43];
        SCY = &MEM[0xFF42];
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
        
        setBG();
        
        for( int y = 0; y < 144; ++y ) {
            for( int x = 0; x < 160; ++x ) {
                unsigned idx = (y+(*SCY))*256 + (x+(*SCX));
                tdraw.drawPixel( x, y, PAL[BG[idx]] );
            }
        }
        
//         for( int y = 0; y < 256; ++y ) {
//             for( int x = 0; x < 256; ++x ) {
//                 tdraw.drawPixel( x, y, (BG[y*256+x]?0:8) );
//             }
//         }
        
        char status[16];
        memset( status, 0, sizeof(status) );
        sprintf( status, "%d", CYCLES );
        tdraw.Status2( status );
        
        tdraw.draw();
        
    }
    
private:
    
    inline void setBG() {
        memset( BG, 0, sizeof(BG) );
        CURSOR = 0;
        bool sgn = ( TSLoc == TLS2 );
        
        for( WORD h = 0; h < 32; ++h ) {
            for( BYTE l = 0; l < 8; ++l ) {
                for( WORD t = 0; t < 32; ++t ) {
                    if( sgn ) {
                        setTileLine( *((SBP)(&MEM[BGLoc+t+32*h])), l );
                    } else {
                        setTileLine( MEM[BGLoc+t+32*h], l );
                    }
                }
            }
        }
    }
    
    inline void setTileLine( int id, WORD line ) {
        for( WORD i = 0; i < 8; ++i ) {
            setTilePixel( id, line, i );
        }
    }    
    
    inline void setTilePixel( int id, WORD line, WORD pix ) {
        unsigned idx = TSLoc+(id*16)+line*2;
        BG[CURSOR] = getColor(GB(&MEM[idx+1],7-pix)<<1 | GB(&MEM[idx],7-pix));
        CURSOR++;
    }
    
    inline BYTE getColor( BYTE idx ) {
        return ( GB(&MEM[0xFF47], idx*2+1)<<1 | GB(&MEM[0xFF47], idx*2 ));
    }
    
    // Draw background layer
    inline void drawBG() {
        
        for( BYTE i = 0; i < 0x29; ++i ) {
            drawTile( 19 );
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
