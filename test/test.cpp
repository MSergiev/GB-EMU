#include "catch.hpp"

#include "../INSTRUCTION_SET.h"
#include "../REGISTERS.h"
#include "../OPERATIONS.h"
// #include <iostream>

#define FLAGS(a,b,c,d)\
REQUIRE(GFZ()==a);\
REQUIRE(GFN()==b);\
REQUIRE(GFH()==c);\
REQUIRE(GFC()==d);

TEST_CASE( "Operations test", "[OPERATIONS]" ) {
    
    MEM = new BYTE[0xFFFF];
    
    SECTION( "LD" ) {
        BYTE a = 10;
        LD( A, &a );
        REQUIRE( *A == a );
        
        WORD addr = 0xFF40;
        LD( &MEM[addr], &a );
        REQUIRE( MEM[addr] == a );
    }
    
    SECTION( "LDHL" ) {
        SP = 0x0FFF;
        SBYTE a = 1;
        LDHL( &a );
        REQUIRE( *HL == 0x1000 );
        FLAGS(0,0,1,0);
        
        SP = 0xFFFF;
        a = 1;
        LDHL( &a );
        REQUIRE( *HL == 0 );
        FLAGS(0,0,1,1);
        
        SP = 0xFFF8;
        a = 2;
        LDHL( &a );
        REQUIRE( *HL == 0xFFFA );
        FLAGS(0,0,0,0);
        
    }
    
    SECTION( "AND" ) {
        *A = 0x5A;
        *L = 0x3F;
        AND( L );
        REQUIRE( *A == 0x1A );
        FLAGS(0,0,1,0);
        
        *A = 0x5A;
        BYTE a = 0x38;
        AND( &a );
        REQUIRE( *A == 0x18 );
        FLAGS(0,0,1,0);
        
        *A = 0x5A;
        *HL = 0xFF00;
        MEM[*HL] = 0;
        AND( &MEM[*HL] );
        REQUIRE( *A == 0 );
        FLAGS(1,0,1,0);
    } 
    
    SECTION( "OR" ) {
        *A = 0x5A;
        OR( A );
        REQUIRE( *A == 0x5A );
        REQUIRE( GFZ() == 0 );
        
        *A = 0x5A;
        BYTE a = 3;
        OR( &a );
        REQUIRE( *A == 0x5B );
        REQUIRE( GFZ() == 0 );
        
        *A = 0x5A;
        *HL = 0xFF00;
        MEM[*HL] = 0x0F;
        OR( &MEM[*HL] );
        REQUIRE( *A == 0x5F );
        REQUIRE( GFZ() == 0 );
    }  
    
    SECTION( "XOR" ) {
        *A = 0xFF;
        XOR( A );
        REQUIRE( *A == 0 );
        REQUIRE( GFZ() == 1 );
        
        *A = 0xFF;
        BYTE a = 0x0F;
        XOR( &a );
        REQUIRE( *A == 0xF0 );
        REQUIRE( GFZ() == 0 );
        
        *A = 0xFF;
        *HL = 0xFF45;
        MEM[*HL] = 0x8A;
        XOR( &MEM[*HL] );
        REQUIRE( *A == 0x75 );
        REQUIRE( GFZ() == 0 );
    }  
    
    SECTION( "CP" ) {
        *A = 0x3C;
        *B = 0x2F;
        CP( B );
        FLAGS(0,1,1,0);
        
        *A = 0x3C;
        BYTE a = 0x3C;
        CP( &a );
        FLAGS(1,1,0,0);
        
        *A = 0x3C;
        *HL = 0xF345;
        MEM[*HL] = 0x40;
        CP( &MEM[*HL] );
        FLAGS(0,1,0,1);
    }    
    
    SECTION( "ADD" ) {
        *A = 0x3C;
        *HL = 0xFFAB;
        MEM[*HL] = 0x12;
        ADD( A, &MEM[*HL] );
        REQUIRE( *A == 0x4E );
        FLAGS(0,0,0,0);
        
        *A = 0x3C;
        BYTE a = 0xFF;
        ADD( A, &a );
        REQUIRE( *A == 0x3B );
        FLAGS(0,0,1,1);
        
        *A = 0x3A;
        *B = 0xC6;
        ADD( A, B );
        REQUIRE( *A == 0 );
        FLAGS(1,0,1,1);
        
        *HL = 0x8A23;
        *BC = 0x0605;
        ADD( HL, BC );
        REQUIRE( *HL == 0x9028 );
        REQUIRE( GFH() == 1 );
        REQUIRE( GFN() == 0 );
        REQUIRE( GFC() == 0 );
        
        *HL = 0x8A23;
        ADD( HL, HL );
        REQUIRE( *HL == 0x1446 );
        REQUIRE( GFH() == 1 );
        REQUIRE( GFN() == 0 );
        REQUIRE( GFC() == 1 ); 
        
        SP = 0xFFF8;
        SBYTE s = 2;
        ADD( &SP, &s );
        REQUIRE( SP == 0xFFFA );
        FLAGS(0,0,0,0);
    }
    
    SECTION( "ADC" ) {
        *A = 0xE1;
        *E = 0x0F;
        SFC( 1 );
        ADC( A, E );
        REQUIRE( *A == 0xF1 );
        FLAGS(0,0,1,0);
        
        *A = 0xE1;
        BYTE a = 0x3B;
        SFC( 1 );
        ADC( A, &a );
        REQUIRE( *A == 0x1D );
        FLAGS(0,0,0,1);
        
        *A = 0xE1;
        a = 0x1E;
        SFC( 1 );
        ADC( A, &a );
        REQUIRE( *A == 0 );
        FLAGS(1,0,1,1);
    }
    
    SECTION( "INC" ) {
        *A = 0xFF;
        INC( A );
        REQUIRE( *A == 0 );
        REQUIRE(GFZ() == 1);
        REQUIRE(GFN() == 0);
        REQUIRE(GFH() == 1);
        
        *HL = 0x1673;
        MEM[*HL] = 0x50;
        INC( &MEM[*HL] );
        REQUIRE( MEM[*HL] == 0x51 );
        REQUIRE(GFZ() == 0);
        REQUIRE(GFN() == 0);
        REQUIRE(GFH() == 0);
        
        *DE = 0x235F;
        INC( DE );
        REQUIRE( *DE == 0x2360 );
    }
    
    SECTION( "SUB" ) {        
        *A = 0x3E;
        *HL = 0xFFAC;
        MEM[*HL] = 0x40;
        SUB( A, &MEM[*HL] );
        REQUIRE( *A == 0xFE );
        FLAGS(0,1,0,1);
        
        *A = 0x3E;
        BYTE a = 0x0F;
        SUB( A, &a );
        REQUIRE( *A == 0x2F );
        FLAGS(0,1,1,0);
        
        *A = 0x3E;
        *E = 0x3E;
        SUB( A, E );
        REQUIRE( *A == 0 );
        FLAGS(1,1,0,0);
    }
    
    SECTION( "SBC" ) {
        *A = 0x3B;
        *H = 0x2A;
        SFC( 1 );
        SBC( A, H );
        REQUIRE( *A == 0x10 );
        FLAGS(0,1,0,0);
        
        *A = 0x3B;
        BYTE a = 0x3A;
        SFC( 1 );
        SBC( A, &a );
        REQUIRE( *A == 0 );
        FLAGS(1,1,0,0);
        
        *A = 0x3B;
        *HL = 0xFA02;
        MEM[*HL] = 0x4F;
        SFC( 1 );
        SBC( A, &MEM[*HL] );
        REQUIRE( *A == 0xEB );
        FLAGS(0,1,1,1);
    }
    
    SECTION( "DEC" ) {
        *L = 0x01;
        DEC( L );
        REQUIRE( *L == 0 );
        REQUIRE(GFZ() == 1);
        REQUIRE(GFN() == 1);
        REQUIRE(GFH() == 0);
        
        *HL = 0x9245;
        MEM[*HL] = 0;
        DEC( &MEM[*HL] );
        REQUIRE( MEM[*HL] == 0xFF );
        REQUIRE(GFZ() == 0);
        REQUIRE(GFN() == 1);
        REQUIRE(GFH() == 1);
        
        *DE = 0x235F;
        DEC( DE );
        REQUIRE( *DE == 0x235E );
    }

    SECTION( "RLCA" ) {
        *A = 0x85;
        RLCA();
        REQUIRE( *A == 0x0B );
        FLAGS(0,0,0,1);
    }
    
    SECTION( "RLA" ) {
        *A = 0x95;
        SFC( 1 );
        RLA();
        REQUIRE( *A == 0x2B );
        FLAGS(0,0,0,1);
    }    
    
    SECTION( "RRCA" ) {
        *A = 0x3B;
        RRCA();
        REQUIRE( *A == 0x9D );
        FLAGS(0,0,0,1);
    }
    
    SECTION( "RRA" ) {
        *A = 0x81;
        SFC( 0 );
        RRA();
        REQUIRE( *A == 0x40 );
        FLAGS(0,0,0,1);
    } 
    
    SECTION( "RLC" ) {
        *A = 0x85;
        RLCA();
        REQUIRE( *A == 0x0B );
        FLAGS(0,0,0,1);
    }
    
    SECTION( "RL" ) {
        *A = 0x95;
        SFC( 1 );
        RLA();
        REQUIRE( *A == 0x2B );
        FLAGS(0,0,0,1);
    }    
    
    SECTION( "RRC" ) {
        *A = 0x3B;
        RRCA();
        REQUIRE( *A == 0x9D );
        FLAGS(0,0,0,1);
    }
    
    SECTION( "RR" ) {
        *A = 0x81;
        SFC( 0 );
        RRA();
        REQUIRE( *A == 0x40 );
        FLAGS(0,0,0,1);
    } 
    
    SECTION( "SLA" ) {
        *D = 0x80;
        SLA( D );
        REQUIRE( *D == 0x00 );
        FLAGS(1,0,0,1);
        
        *HL = 0x0043;
        MEM[*HL] = 0xFF;
        SLA( &MEM[*HL] );
        REQUIRE( MEM[*HL] == 0xFE );
        FLAGS(0,0,0,1);
    }
    
    SECTION( "SRA" ) {
        *A = 0x8A;
        SRA( A );
        REQUIRE( *A == 0xC5 );
        FLAGS(0,0,0,0);
        
        *HL = 0x6199;
        MEM[*HL] = 0x01;
        SRA( &MEM[*HL] );
        REQUIRE( MEM[*HL] == 0x00 );
        FLAGS(1,0,0,1);
    }  
    
    SECTION( "SRL" ) {
        *A = 0x01;
        SRL( A );
        REQUIRE( *A == 0x00 );
        FLAGS(1,0,0,1);
        
        *HL = 0x2893;
        MEM[*HL] = 0xFF;
        SRL( &MEM[*HL] );
        REQUIRE( MEM[*HL] == 0x7F );
        FLAGS(0,0,0,1);
    }
    
    SECTION( "SWAP" ) {
        *A = 0x00;
        SRL( A );
        REQUIRE( *A == 0x00 );
        FLAGS(1,0,0,0);
        
        *HL = 0x9233;
        MEM[*HL] = 0xF0;
        SWAP( &MEM[*HL] );
        REQUIRE( MEM[*HL] == 0x0F );
        FLAGS(0,0,0,0);
    }  
    
    SECTION( "BIT" ) {
        *A = 0x80;
        SFC(0);
        BIT( 7, A );
        FLAGS(0,0,1,0);
        
        *L = 0xEF;
        BIT( 4, L );
        FLAGS(1,0,1,0);
        
        *HL = 0x34EF;
        MEM[*HL] = 0xFE;
        BIT( 0, &MEM[*HL] );
        FLAGS(1,0,1,0);
        
        *HL = 0x34EF;
        MEM[*HL] = 0xFE;
        BIT( 1, &MEM[*HL] );
        FLAGS(0,0,1,0);
    }   
    
    SECTION( "SET" ) {
        *A = 0x80;
        SET( 3, A );
        REQUIRE( *A == 0x88 );
        
        *L = 0x3B;
        SET( 7, L );
        REQUIRE( *L == 0xBB );
        
        *HL = 0xBF34;
        MEM[*HL] = 0x00;
        SET( 3, &MEM[*HL] );
        REQUIRE( MEM[*HL] == 0x08 );
    } 
    
    SECTION( "RES" ) {
        *A = 0x80;
        RES( 7, A );
        REQUIRE( *A == 0x00 );
        
        *L = 0x3B;
        RES( 1, L );
        REQUIRE( *L == 0x39 );
        
        *HL = 0xAAAA;
        MEM[*HL] = 0xFF;
        RES( 3, &MEM[*HL] );
        REQUIRE( MEM[*HL] == 0xF7 );
    }      
    
    SECTION( "JP" ) {
        WORD addr = 0x8000;
        PC = 0;
        JP( 1, &addr );
        REQUIRE( PC == 0x8000-1 );
        
        addr = 0xFFFF;
        JP( 0, &addr );
        REQUIRE( PC == 0x8000-1 );
    }  
    
    SECTION( "JR" ) {
        BYTE a = 0b11111101;
        PC = 0x0004;
        JR( 1, &a );
        REQUIRE( PC == 0x0002-1 );
        
        a = 4;
        PC = 12;
        JR( 1, &a );
        REQUIRE( PC == 16-1 );
    }
    
    SECTION( "DAA" ) {
        *A = 0x45;
        *B = 0x38;
        ADD( A, B );
        DAA();
        REQUIRE( *A == 0x83 );
        REQUIRE( GFC() == 0 );
    }  
    
    delete[] MEM;
    
}
