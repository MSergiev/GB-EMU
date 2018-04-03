#ifndef REGISTERS_H
#define REGISTERS_H

// 8-bit architecture
#define ARCH 8
// Clock speed
#define CLK 4.2

// Memory map
#define ROM_BANK0	0x0000
#define ROM_BANK1	0x4000
#define VRAM		0x8000
#define ERAM		0xA000
#define WRAM0		0xC000
#define WRAM1		0xD000
#define ECHO		0xE000
#define SAT			0xFE00
#define NA			0xFF00
#define HRAM		0xFF80
#define IER			0xFFFF

typedef unsigned char BYTE;
typedef char SBYTE;
typedef unsigned short WORD;
typedef short SWORD;
typedef bool FLAG;
typedef BYTE* BP;
typedef SBYTE* SBP;
typedef WORD* WP;

#define CTBP(x) reinterpret_cast<BYTE*>(x)
#define CTWP(x) reinterpret_cast<WORD*>(x)
#define BTW(x,y) (x<<8)|y
#define WTB(x,y) 

// CPU running flag
static FLAG ACT;

// Interrupts enabled flag
static FLAG ITR;

// Registers
static BYTE REG[8];
static WORD PC, SP;
static const BYTE FZ = 7, FN = 6, FH = 5, FC = 4;

// BYTE register pointers
static const BP A = &REG[1];
static const BP F = &REG[0];
static const BP B = &REG[3];
static const BP C = &REG[2];
static const BP D = &REG[5];
const BP E = &REG[4];
const BP H = &REG[7];
const BP L = &REG[6];

// WORD register pointers
const WP AF = CTWP(&REG[0]);
const WP BC = CTWP(&REG[2]);
const WP DE = CTWP(&REG[4]);
const WP HL = CTWP(&REG[6]);

// Memory
static BYTE MEM[65535];


// Helper functions


static inline void SET ( BYTE i, BP a ) {
    (*a) |= (BYTE)((1<<i));
}

static inline void RES ( BYTE i, BP a ) {
    (*a) &= (BYTE)(~(1<<i));
}

// Set bit
static inline void SB( BP a, BYTE i, bool v ) {
    if(v) SET(i, a);
    else RES(i, a);
}

// Get bit
static inline bool GB( BP a, BYTE i ) { 
    return (1 & ((*a)>>i)); 
}

// Get most significant nibble
static inline BYTE GMSN( BYTE a ) {
    return a>>4;
}

// Get least significant nibble
static inline BYTE GLSN( BYTE a ) {
    return a&0xF;
}

// Get most significant byte
static inline BYTE GMSB( WORD a ) {
    return (BYTE)(a>>8);
}

// Get least significant byte
static inline BYTE GLSB( WORD a ) {
    return (BYTE)(a&0xFF);
}

// Set most significant byte
static inline void SMSB( WORD& a, BYTE v ) {
    a = (v<<8) | GLSB(a);
}

// Set least significant byte
static inline void SLSB( WORD& a, BYTE v ) {
    a = (GMSB(a)<<8) | v;
}

#endif
