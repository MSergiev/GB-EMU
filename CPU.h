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
    unsigned long cycles;
    
public:
    
    // Constructor
    CPU() {
        debugging = false;
        debugMode = 200;
        cycles = 0;
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
        
        std::cout << "Loading ROM" << std::endl;
        
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
        
        std::cout << "ROM loaded" << std::endl;
        
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
            if( debugging ) {
                parse( &MEM[PC] );
                debug();
            }
            process( MEM[PC] );
            cycles++;
        }
    }
    
private:
    
    inline void dumpReg() {
        std::cout << "  PC = "; printHex(PC); std::cout << std::endl;
        std::cout << "  AF = "; printHex(*AF); std::cout << std::endl;
        std::cout << "  BC = "; printHex(*BC); std::cout << std::endl;
        std::cout << "  DE = "; printHex(*DE); std::cout << std::endl;
        std::cout << "  HL = "; printHex(*HL); std::cout << std::endl;
        std::cout << "  SP = "; printHex(SP); std::cout << std::endl;
    }

    inline void parse( BP inst, bool pc = true ) {
        
        BYTE val[] = {*inst, 0, 0};
        BYTE len = 0;
        
        if(pc) std::cout << std::hex << "$ 0x" << std::setfill('0') << std::setw(6) << +PC;
        bool isExt = (val[0] == 0xCB);
        
        if( isExt ) {
            len = EXT_INST_LEN;
            memcpy( val, inst, len );
            std::cout << std::setw(13) << std::setfill(' ') << EXT_INST[val[1]];
        } else {
            len = INST_LEN[val[0]];
            memcpy( val, inst, len );
            std::cout << std::setw(13)  << std::setfill(' ')<< INST[val[0]];
        }
        
        std::cout << std::hex << "   [" << std::setfill('0');
        for( BYTE i = 0; i < len; ++i ) {
            std::cout << std::setw(2) << +(BYTE)val[i] << (i<len-1 ? " " : "");
        }
        std::cout << "]   "; 
        for( BYTE i = 0; i < (3-len); ++i ) std::cout << "   ";
        std::cout << (isExt?EXT_INST_CMNT[val[1]]:INST_CMNT[val[0]]) << std::endl;
//         std::cout << std::endl;
        
    }
    
    inline void printHex( WORD a ) {
        std::cout << "0x" << std::setfill('0') << std::setw(4) << std::hex << +a;
    }
    
private:    // Debugging
    
    bool debugging;
    unsigned long debugParam;
    BYTE debugMode;
    std::set<WORD> breakpoints;
    
public:
    
    inline bool enableDebug() {
        
        debugging = true;
        debugMode = 1;
//         while( debugMode > 1 ) {
//             std::cout << "\nEnter option:" << std::endl;
//             std::cout << " 0 - Run" << std::endl;
//             std::cout << " 1 - Step by step" << std::endl;
//             std::cout << " Q - Quit" << std::endl;
//             std::cout << "- Input: ";
//             std::cin >> debugMode;
//             if( debugMode == 'q' or debugMode == 'Q' ) {
//                 return false;
//             }
//             debugMode -= '0';
//             if( debugMode > 1 ) {
//                 std::cerr << "- ERROR: Invalid option " << +debugMode << std::endl;
//             }
//         }
        
        return true;
        
    }
    
private:
    
    inline void debug() {
        
        switch( debugMode ) {
            // Continuous
            case 0: {
                if( breakpoints.find(PC) != breakpoints.end() ) { 
                    break;
                }
                return;
            }
            // Step by step
            case 1: break;
            // Instruction
            case 2: {
                break;
            }
            // Memory address
            case 3: {
                if( MEM[PC] == debugParam ) {
                    break;
                }
                return;
            }
            // Number of cycles
            case 4: {
                if( cycles == debugParam ) {
                    break;
                }
                return;
            }
        }
        
        BYTE sel = 0;
        while( !sel ) {
            std::cout << "> ";
            std::cin >> sel;
            switch( sel ) {
                // Continue
                case 'C': case 'c': {
                    debugMode = 0;
                    break;
                }
                // Step over
                case 'S': case 's': {
                    debugMode = 1;
                    break;
                }    
                // Number of cycles
                case 'F': case 'f': {
                    unsigned c = 0;
                    std::cout << "- Enter number of cycles: ";
                    std::cin >> std::dec >> c;
                    debugParam = cycles + c;
                    debugMode = 4;
                    break;
                }
                // Memory address
                case 'O': case 'o': {
                    unsigned addr = 0;
                    std::cout << "- Enter instruction: 0x";
                    std::cin >> std::hex >> addr;
                    if( addr < 0xFFFF ) {
                        std::cout << "- Running until instruction "; printHex( (WORD)addr ); 
                        std::cout << std::endl;
                        debugParam = (WORD)addr;
                        debugMode = 3;
                    } else {
                        std::cout << "- ERROR: Address overflow" << std::endl;
                    }
                    break;
                }
                // Add breakpoint
                case 'B': case 'b': {
                    unsigned addr = 0;
                    std::cout << "- Enter instruction number: 0x";
                    std::cin >> std::hex >> addr;
                    if( addr < 0xFFFF ) {
                        std::cout << "- Adding breakpoint at "; printHex( (WORD)addr ); 
                        std::cout << std::endl;
                        breakpoints.insert( (WORD)addr );
                    } else {
                        std::cout << "- ERROR: Address overflow" << std::endl;
                    }
                    sel = 0;
                    break;
                }
                // List breakpoints
                case 'L': case 'l': {
                    std::cout << "- Breakpoints:" << std::endl;;
                    std::set<WORD>::iterator i;
                    for( i = breakpoints.begin(); i != breakpoints.end(); ++i ) {
                        std::cout << "  "; printHex(*i); std::cout << " "; parse( &MEM[*i], false );
                    }
                    sel = 0;
                    break;
                }
                // Remove breakpoint
                case 'D': case 'd': {
                    unsigned addr = 0;
                    std::cout << "- Enter instruction number: 0x";
                    std::cin >> std::hex >> addr;
                    if( addr < 0xFFFF ) {
                        if( breakpoints.find(addr) != breakpoints.end() ) {
                            std::cout << "- Removing breakpoint at "; printHex( (WORD)addr ); 
                            std::cout << std::endl;
                            breakpoints.erase( addr );
                        } else {
                            std::cout << "- Breakpoint "; printHex( (WORD)addr ); 
                            std::cout << " does not exist" << std::endl;
                        }
                    } else {
                        std::cout << "- ERROR: Address overflow" << std::endl;
                    }
                    sel = 0;
                    break;
                }
                // Memory dump
                case 'M': case 'm': {
                    WORD addr = 0;
                    std::cout << "- Enter address: 0x";
                    std::cin >> std::hex >> addr;
                    if( addr < 0xFFFF ) {
                        std::cout << "- Value at address "; printHex( addr );
                        std::cout << " = "; printHex( MEM[addr] );
                        std::cout << std::endl;
                    } else {
                        std::cout << "- ERROR: Address overflow" << std::endl;
                    }
                    sel = 0;
                    break;
                }
                // Quit
                case 'Q': case 'q': {
                    ACT = false;
                    break;
                }
                // Registry dump
                case 'R': case 'r': {
                    dumpReg();
                    sel = 0;
                    break;
                }
                // Help
                case 'H': case 'h': {
                    std::cout << "  C - Continue" << std::endl;
                    std::cout << "  S - Step over" << std::endl;
                    std::cout << "  F - Run fixed number of cycles" << std::endl;
                    std::cout << "  O - Run until instruction" << std::endl;
                    std::cout << "  L - List breakpoints" << std::endl;
                    std::cout << "  B - Add breakpoint" << std::endl;
                    std::cout << "  D - Delete breakpoint" << std::endl;
                    std::cout << "  R - Dump registry data" << std::endl;
                    std::cout << "  M - Dump data from memory" << std::endl;
                    std::cout << "  Q - Quit" << std::endl;
                    sel = 0;
                    break;
                }
                // Invalid command
                default: {
                    std::cout << "- ERROR: Invalid command. Press h for help" << std::endl;
                    sel = 0;
                    break;
                }
            }
        }
        
    }
    
};

#endif
