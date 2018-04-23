#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <fstream>
#include <set>
#include "INSTRUCTION_SET.h"
#include "REGISTERS.h"

class CPU {
    
private:
    
#include "INSTRUCTIONS.h"
    
    unsigned long OldCYCLES;
    
public:
    
    // Constructor
    CPU() {
        debugging = false;
        verbose = false;
        debugMode = 200;
        memset( REG, 0, sizeof(REG) );
        PC = 0; SP = 0;
        OldCYCLES = 0;
    }
    
    // Destructor
    ~CPU() {
        delete[] MEM;
    }
    
    bool loadROM( const char* filename ) {
        std::ifstream file( filename, std::ios::binary );
        if( !file ) {
            std::cerr << "ERROR: " << filename << " does not exist!" << std::endl;
            return false;
        }
        
        std::cout << "Loading " << filename << std::endl;
        
        std::streampos fsize = 0;
        fsize = file.tellg();
        file.seekg( 0, std::ios::end );
        fsize = file.tellg() - fsize;
        file.seekg( 0, std::ios::beg );
        file.clear();
        
        std::cout << "Size: " << fsize << std::endl;  
        
        if( MEM == nullptr ) {
            unsigned SIZE = fsize;
            if( SIZE < 0xFFFF ) SIZE = 0xFFFF;
            MEM = new BYTE[SIZE];
//             memset( MEM, 0xFF, SIZE );
        }
        
        for( long idx = 0; idx < fsize; ++idx ) {
            char val;
            file.seekg( idx );
            file.read( &val, 1 );
            if( file.eof() ) break;
            MEM[idx] = val;
        }
        
        std::cout << filename << " loaded" << std::endl;
        
        file.close();
        
        return true;
    }    
    
    void run() {
        if( ACT ) {
            
            if( CYCLES >= OldCYCLES + SCANLINE ) {
                OldCYCLES = CYCLES;
                MEM[0xFF44]++;
                if( MEM[0xFF44] > SCANCOL ) MEM[0xFF44] = 0;
            }
            
//             if( MEM[0xFF02] == 0x81 ) {
//                 SERIAL.push_back(MEM[0xFF01]);
//                 MEM[0xFF02] = 0;
//                 
//             }
//             MEM[0xFF44] = 0x90;
            if( debugging ) {
                if( verbose ) dumpReg();
                parse( &MEM[PC] );
                debug();
            }
            process( MEM[PC] );
        }
    }
    
private:
    
    void printHex( WORD a ) {
        std::cout << "0x" << std::setfill('0') << std::setw(4) << std::hex  << std::uppercase << +a;
    }
    
private:    // Debugging
    
    #include "DEBUG.h"
    
};

#endif
