#ifndef DEBUG_H
#define DEBUG_H

    bool debugging;
    unsigned long debugParam;
    bool verbose;
    BYTE debugMode;
    std::set<WORD> breakpoints;
    
    bool BG[65536];
    unsigned CURSOR;
    
public:
    
    inline bool enableDebug() {
        debugging = true;
        debugMode = 1;        
        return true;
    }
    
private:
    
    inline void debug() {
        
        switch( debugMode ) {
            // Continuous
            case 0: {
                if( breakpoints.find(PC) != breakpoints.end() ) {
                    std::cout << "* ";
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
                    std::cout << "# ";
                    break;
                }
                return;
            }
            // Number of cycles
            case 4: {
                if( CYCLES >= debugParam ) {
                    std::cout << "! ";
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
                    debugParam = CYCLES + c;
                    debugMode = 4;
                    break;
                }
                
                // Run fixed number of frames
                case 'A': case 'a': {
                    unsigned c = 0;
                    std::cout << "- Enter number of frames: ";
                    std::cin >> std::dec >> c;
                    debugParam = CYCLES + (c*FRAME);
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
                
                // Registry dump
                case 'R': case 'r': {
                    dumpReg();
                    sel = 0;
                    break;
                }
                
                // Preview tile
                case 'T': case 't': {
                    WORD addr = 0;
                    std::cout << "- Enter tile id: 0x";
                    std::cin >> std::hex >> addr;
                    if( addr < 0xFFFF ) {
                        printTile( addr );
                    } else {
                        std::cout << "- ERROR: Address overflow" << std::endl;
                    }
                    sel = 0;
                    break;
                } 
                
                // Preview graphics
                case 'P': case 'p': {
                    preview();
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
                
                // Dump memory to file
                case 'X': case 'x': {
                    std::ofstream file( "mem.txt" );
                    for( unsigned i = 0x0000; i <= 0xFFFF; ++i ) {
                        if( i%0x10 == 0 ) {
                            if( i != 0 ) file << std::endl;
                            file << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << +i;
                            file << ":"  << std::hex << std::uppercase << std::setw(4) << +(i+0xF) << " ";
                        }
                        file << std::hex  << std::uppercase << std::setw(2) << +MEM[i] << " ";
                    }
                    file.close();
                    std::cout << "- Memory dumped to mem.txt" << std::endl;
                    sel = 0;
                    break;
                }
                
                // Verbose mode
                case 'V': case 'v': {
                    verbose = !verbose;
                    std::cout << "- Verbose mode " << ( verbose ? "enabled" : "disabled" ) << std::endl;
                    sel = 0;
                    break;
                }
                
                // Quit
                case 'Q': case 'q': {
                    ACT = false;
                    break;
                }
                
                // Help
                case 'H': case 'h': {
                    std::cout << "  C - Continue" << std::endl;
                    std::cout << "  S - Step over" << std::endl;
                    std::cout << "  F - Run fixed number of cycles" << std::endl;
                    std::cout << "  A - Run fixed number of frames" << std::endl;
                    std::cout << "  O - Run until instruction" << std::endl;
                    std::cout << "  L - List breakpoints" << std::endl;
                    std::cout << "  B - Add breakpoint" << std::endl;
                    std::cout << "  D - Delete breakpoint" << std::endl;
                    std::cout << "  R - Dump registry data" << std::endl;
                    std::cout << "  T - Preview tile" << std::endl;
                    std::cout << "  P - Preview graphics" << std::endl;
                    std::cout << "  M - Read data from memory" << std::endl;
                    std::cout << "  X - Dump memory to file" << std::endl;
                    std::cout << "  V - Verbose mode" << std::endl;
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
    
    inline void dumpReg() {
        std::cout << "  LCDC = "; printHex(MEM[0xFF40]); std::cout << std::endl;
        std::cout << "  SCL = "; printHex(MEM[0xFF44]); std::cout << std::endl;
        std::cout << "  SCX = "; printHex(MEM[0xFF43]); std::cout << std::endl;
        std::cout << "  SCY = "; printHex(MEM[0xFF42]); std::cout << std::endl << std::endl;
        std::cout << "  FZ = " << GFZ();
        std::cout << "  FN = " << GFN();
        std::cout << "  FH = " << GFH();
        std::cout << "  FC = " << GFC() << std::endl;
        std::cout << "  PC = "; printHex(PC); 
        std::cout << " ["; printHex(*(WP)(&MEM[PC])); std::cout << "]" << std::endl;
        std::cout << "  AF = "; printHex(*AF); 
        std::cout << " ["; printHex(*(WP)(&MEM[*AF])); std::cout << "]" << std::endl;
        std::cout << "  BC = "; printHex(*BC); 
        std::cout << " ["; printHex(*(WP)(&MEM[*BC])); std::cout << "]" << std::endl;
        std::cout << "  DE = "; printHex(*DE); 
        std::cout << " ["; printHex(*(WP)(&MEM[*DE])); std::cout << "]" << std::endl;
        std::cout << "  HL = "; printHex(*HL); 
        std::cout << " ["; printHex(*(WP)(&MEM[*HL])); std::cout << "]" << std::endl;
        std::cout << "  SP = "; printHex(SP); 
        std::cout << " ["; printHex(*(WP)(&MEM[SP])); std::cout << "]" << std::endl;
    }

    inline void parse( BP inst, bool pc = true ) {
        
        BYTE val[] = {*inst, 0, 0};
        BYTE len = 0;
        
        if(pc) std::cout << "$ " << std::dec << CYCLES;
        std::cout << std::hex << std::uppercase << " 0x" << std::setfill('0') << std::setw(6) << +PC;
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
        
        std::cout << std::hex << std::uppercase  << "   [" << std::setfill('0');
        for( BYTE i = 0; i < len; ++i ) {
            std::cout << std::setw(2) << +(BYTE)val[i] << (i<len-1 ? " " : "");
        }
        std::cout << "]   "; 
        for( BYTE i = 0; i < (3-len); ++i ) std::cout << "   ";
        if( verbose ) {
            std::cout << (isExt?EXT_INST_CMNT[val[1]]:INST_CMNT[val[0]]) << std::endl;
        } else {
            std::cout << std::endl;
        }
        
    }
    
    inline void preview() {
        
        WORD SCX = MEM[0xFF43];
        WORD SCY = MEM[0xFF42];
        std::cout << "LCDC: "; printHex(MEM[0xFF40]); std::cout << std::endl;
        std::cout << "SCX: "; printHex(SCX); std::cout << std::endl;
        std::cout << "SCY: "; printHex(SCY); std::cout << std::endl;
        
        setBG();
        
        for( int h = SCY; h < SCY+144; ++h ) {
            for( int w = SCX; w < SCX+160; ++w ) {
                std::cout << (BG[h*256+w]?'X':' ');
            }
            std::cout << std::endl;
        }
        
//         for( WORD h = 0; h < 18; ++h ) {
//             for( BYTE l = 0; l < 8; ++l ) {
//                 for( WORD t = 0; t < 20; ++t ) {
//                     printTileLine( MEM[0x9800+(t+SCX)+32*(h+SCY)], l );
//                 }
//                 std::cout << std::endl;
//             }
//         }
        
    }
    
    inline void setBG() {
        memset( BG, 0, sizeof(BG) );
        CURSOR = 0;
        
        for( WORD h = 0; h < 32; ++h ) {
            for( BYTE l = 0; l < 8; ++l ) {
                for( WORD t = 0; t < 32; ++t ) {
                    setTileLine( MEM[0x9800+t+32*h], l );
                }
            }
        }
    }
    
//     inline void printBG() {
//         for( WORD h = 0; h < 32; ++h ) {
//             for( BYTE l = 0; l < 8; ++l ) {
//                 for( WORD t = 0; t < 32; ++t ) {
//                     printTileLine( MEM[0x9800+t+32*h], l );
//                 }
//                 std::cout << std::endl;
//             }
//         }
//     }
    
    inline void printTileLine( WORD id, WORD line ) {
        WORD SCX = MEM[0xFF43];
        WORD SCY = MEM[0xFF42];
        for( WORD i = 0; i < 8; ++i ) {
            printTilePixel( id, line, i );
        }
    }  
    
    inline void setTileLine( WORD id, WORD line ) {
        for( WORD i = 0; i < 8; ++i ) {
            setTilePixel( id, line, i );
        }
    }    
    
    inline void printTilePixel( WORD id, WORD line, WORD pix ) {
        std::cout << (GB(&MEM[0x8000+(id*16)+line*2],7-pix) ? '1': ' ');
    }   
    
    inline void setTilePixel( WORD id, WORD line, WORD pix ) {
        BG[CURSOR] = GB(&MEM[0x8000+(id*16)+line*2],7-pix);
        CURSOR++;
    }
    
//     inline void printPixel( WORD x, WORD y ) {
//         printTilePixel();
//     }
    
    inline void printTile( WORD id ) {
        for( WORD i = 0; i < 8; ++i ) {
            printTileLine( id, i );
            std::cout << std::endl;
        }
    }    


#endif
