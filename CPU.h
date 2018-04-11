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
    
public:
    
    // Constructor
    CPU() {
        debugging = false;
        verbose = false;
        debugMode = 200;
        memset( REG, 0, sizeof(REG) );
        memset( MEM, 0xFF, sizeof(MEM) );
        PC = 0; SP = 0;
    }
    
    // Destructor
    ~CPU() {}
    
    inline bool loadROM( const char* filename ) {
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
        
        for( unsigned long idx = 0; idx < fsize; ++idx ) {
            char val;
            file.seekg( idx );
            file.read( &val, 1 );
            if( file.eof() ) break;
            MEM[idx] = val;
        }
        
        std::cout << filename << " loaded" << std::endl;
        
        file.close();
        
        // Memory fixes
        memcpy( &MEM[0x104], &MEM[0x00A8], 48 );
        MEM[0xFF44] = 144;
        memset( &MEM[0x134], 0, (0x14D-0x134) );
        MEM[0xFF44] = 0xE7;
        
        ACT = true;
        return true;
    }
    
    inline void run() {
        if( ACT ) {
            MEM[0xFF44] = 0x90;
            if( debugging ) {
                if( verbose ) dumpReg();
                parse( &MEM[PC] );
                debug();
            }
            process( MEM[PC] );
        }
    }
    
private:
    
    inline void printHex( WORD a ) {
        std::cout << "0x" << std::setfill('0') << std::setw(4) << std::hex  << std::uppercase << +a;
    }
    
private:    // Debugging
    
    #include "DEBUG.h"
    
};

#endif
