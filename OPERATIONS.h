#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <fstream>
// #include "REGISTERS.h"



// Flag manipulators



// Set zero flag
inline void SFZ( bool v ) {
    SB( F, FZ, v );
}

// Set subtraction flag
inline void SFN( bool v ) {
    SB( F, FN, v );
}

// Set half carry flag
inline void SFH( bool v ) {
    SB( F, FH, v );
}

// Set carry flag
inline void SFC( bool v ) {
    SB( F, FC, v );
}

// Get zero flag
inline bool GFZ() {
    return GB( F, FZ );
}

// Get subtraction flag
inline bool GFN() {
    return GB( F, FN );
}

// Get half carry flag
inline bool GFH() {
    return GB( F, FH );
}

// Get carry flag
inline bool GFC() {
    return GB( F, FC );
}


// Calculate zero flag
inline void CFZ( BYTE a ) {
    SFZ( a==0 );
}

// Calculate half carry flag
inline void CFH( BYTE a, BYTE b ) {
    SFH( (((a&0xF) + (b&0xF)) > 0xF) );
}

// Calculate carry flag
inline void CFC( BYTE a, BYTE b ) {
    SFC( (((WORD)a + (WORD)b) > 0xFF) );
}



// Pointer manipulators



// Get immediate byte pointer
inline BP BPIM() {
    BP v = &MEM[PC+1];
    PC++;
    return v;
}

// Get immediate signed byte pointer
inline SBP SBPIM() {
    SBP v = (SBP)&MEM[PC+1];
    PC++;
    return v;
}

// Get immediate word pointer
inline WP WPIM() {
    WP v = (WP)&MEM[PC+1];
    PC += 2;
    return v;
}




// CPU operations




inline void NOP () {
}

inline void STOP () {
    ACT = 0;
}

inline void HALT () {
    ACT = 0;
}

inline void LD ( BP a, BP b ) {
    (*a) = (*b);
}

inline void LD ( WP a, WP b ) {
    (*a) = (*b); 
}

inline void LD ( WP a, BP b ) {
    (*a) = (*b); 
}

inline void LD ( BP a, WP b ) {
    (*a) = (*b); 
    (*a) = (GLSN(*a)<<4)|GMSN(*a); 
}

inline void LDHL ( SBP a ) {
    WORD sp = SP+(*a);
    (*HL) = sp; 
    SFZ(0); SFN(0); 
    SFH( (SP&0xF + (*a)&0xF > 0xFFF) ); 
    SFC( ((int)SP + (int)(*a) > 0xFFFF) );
}

inline void INC ( WP a ) { 
    (*a)++; 
}

inline void INC ( BP a ) { 
    (*a)++; 
    CFZ(*a); SFN(0); CFH(*a,1);
}

inline void DEC ( WP a ) { 
    (*a)--; 
}

inline void DEC ( BP a ) { 
    (*a)--; 
    CFZ(*a); SFN(1); 
    SFH( ((int)(((*a)+1)&0xF) - 1) < 0 );
}

inline void ADD ( BP a, BP b ) { 
    (*a) += (*b); 
    CFZ(*a); SFN(0); CFH(*a,*b); CFC(*a,*b);
}

inline void ADD ( WP a, WP b ) { 
    (*a) += (*b); 
    SFN(0); 
    SFH( (SP&0xF + (*a)&0xF > 0xFFF) ); 
    SFC( ((int)SP + (int)(*a) > 0xFFFF) );
}

inline void ADD ( WP a, SBP b ) { 
    (*a) += (*b); 
    SFZ(0); SFN(0); 
    SFH( ((*a)&0xF + (*b)&0xF) > 0xF ); 
    SFC( ((int)(*a) + (int)(*b)) > 0xFFFF );
}

inline void ADC ( BP a, BP b ) { 
    ADD( a, b );
    (*a) += GFC();
    CFZ(*a); SFN(0); CFH(*a,*b); CFC(*a,*b);
}

inline void SUB ( BP a, BP b ) { 
    (*a) -= (*b); 
    CFZ(*a); SFN(1);
    SFH( ((((int)(*a))&0xF) - ((int)(*b)&0xF)) < 0 );
    SFC( ((((int)(*a))&0xFF) - ((int)(*b)&0xFF)) < 0 );
}

inline void SBC ( BP a, BP b ) { 
    SUB( a, b );
    (*a) -= GFC(); 
    CFZ(*a); SFN(1); 
    SFH( ((((int)(*a))&0xF) - (((int)(*b))&0xF)) < 0 );
    SFC( ((((int)(*a))&0xFF) - (((int)(*b))&0xFF)) < 0 );
}

inline void RLC ( BP a ) {
    SFC((((*a)&128)>>7));
    (*a) = ((*a)<<1)|FC;
    SFZ(0); SFN(0); SFH(0); SFC(((*a)&1));
}

inline void RL ( BP a ) {
    bool c = GFC();
    SFC((((*a)&128)>>7));
    (*a) = ((*a)<<1)|c;
    if( a!=A ) CFZ(*a); 
    SFN(0); SFH(0);
}

inline void RRC ( BP a ) {
    SFC((*a)&1);
    (*a) = ((*a)>>1)|(FC<<7);
    SFZ(0); SFN(0); SFH(0); SFC(((*a)&128)>>8);
}

inline void RR ( BP a ) {
    bool c = GFC();
    SFC((*a)&1);
    (*a) = ((*a)>>1)|(c<<7);
    if( a!=A ) CFZ(*a); 
    SFN(0); SFH(0);
}

inline void CP ( BP a ) { 
    SFZ((*A)==(*a)); SFN(1); 
    SFH( ((((int)(*A))&0xF) - ((int)(*a)&0xF)) < 0 );
    SFC( ((((int)(*A))&0xFF) - ((int)(*a)&0xFF)) < 0 );
}

inline void AND ( BP a ) { 
    (*A) &= (*a); 
    CFZ(*A); SFN(0); SFH(1); SFC(0); 
}

inline void OR ( BP a ) { 
    (*A) |= (*a); 
    CFZ(*A); SFN(0); SFH(0); SFC(0); 
}

inline void XOR ( BP a ) { 
    (*A) ^= (*a); 
    CFZ(*A); SFN(0); SFH(0); SFC(0); 
}

inline void JR ( FLAG f, BP a ) {
    if( f ) {
        PC = PC + (SBYTE)(*a);
        CYCLES += JUMP_TRUE;
    }
}

inline void JP ( FLAG f, WP a ) {
    if( f ) {
        PC = (*a) - 1;
        CYCLES += JUMP_TRUE;
    }
}

inline void PUSH ( WP a ) {
    SP--; 
    MEM[SP] = GMSB(*a);
    SP--;
    MEM[SP] = GLSB(*a);
}

inline void POP ( WP a ) {
    SLSB( *a, MEM[SP] );
    SP++; 
    SMSB( *a, MEM[SP] );
    SP++;
}
    
inline void RST ( BYTE a ) {
    PUSH( &PC );
    PC = a;
}

inline void CALL ( FLAG f ) { 
    if( f ) {
        WORD val = PC+3;
        PUSH( &val );
        memcpy( &PC, &MEM[PC+1], 2 );
        PC--;
        CYCLES += CALL_RET_TRUE;
    }
}   

inline void RET ( FLAG f ) { 
    if( f ) {
        POP( &PC );
        PC--;
        CYCLES += CALL_RET_TRUE;
    }
}   

inline void RETI () { 
    EI();
    RET( 1 );
}

inline void EI () { 
    ITR = 1;
}

inline void DI () { 
    ITR = 0;
}

inline void DAA() {
    if( GLSN(*A) > 9 or FH ) {
        (*A)+=0x06;
    }
    if( GMSN(*A) > 9 or FC ) {
        (*A)+=0x60;
        SCF();
    } else {
        CCF();
    }
    CFZ(*A); SFH(0); CFC(*A, *A); 
}
  
inline void CPL() {
    (*A) = ~(*A);
    SFH(1); SFN(1);
}
  
inline void SCF() {
    SFN(0); SFH(0); SFC(1);
}   

inline void CCF() {
    SFN(0); SFH(0); SFC(!GFC());
}

inline void BIT ( BYTE i, BP a ) { 
    SFZ(!GB(a,i)); SFN(0); SFH(1); 
}

inline void SWAP ( BP a ) {
    (*a) = (GLSN(*a)<<4)|GMSN(*a); 
    CFZ(*a); SFN(0); SFH(0); SFC(0); 
}

inline void SLA ( BP a ) {
    SFC(((*a)&128)>>7);
    (*a) <<= 1;
    CFZ(*a); SFN(0); SFH(0);
}

inline void SRA ( BP a ) {
    SFC((*a)&1);
    BYTE b = (*a)&128;
    (*a) >>= 1;
    (*a) |= b;
    CFZ((*a)); SFN(0); SFH(0);
}

inline void SRL ( BP a ) {
    SFC((*a)&1);
    (*a) >>= 1;
    CFZ(*a); SFN(0); SFH(0);
}

#endif
