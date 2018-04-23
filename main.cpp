#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include "CPU.h"
#include "DISPLAY.h"

const char* filename = "rom.gb";

int main( int argc, const char* argv[] ) {
    
    CPU cpu;

    bool debug = false;
    
    if( argc == 3 ) {
        if( argv[2][1] == 'd' ) {
            std::cout << "Debugging enabled" << std::endl;
            if( !cpu.enableDebug() ) return 0;
            debug = true;
        } else {
            std::cerr << "Invalid argument. Use -d for debugging" << std::endl;
            return -1;
        }
    }   
    
    if( argc >= 2 ) {
        if( !cpu.loadROM( argv[1] ) ) return -1;
    }
    
    if( !cpu.loadROM( "bios.gb" ) ) return -1;
    
    DISPLAY display;
    
    // Memory fixes
//     memcpy( &MEM[0x104], &MEM[0x00A8], 48 );
//     MEM[0xFF44] = 144;
//     memset( &MEM[0x134], 0, (0x14D-0x134) );
//     MEM[0xFF44] = 0xE7;
    
    if( !debug ) display.init();
    
    ACT = true;
    
    while(1) {   
        if( !ACT and debug ) break;
          
        
        if( !debug ) {
            display.draw();
            
            int input = getch();
            if( input == 'q' or input == 'Q' or input == KEY_EXIT ) break;
        }
        
        cpu.run();
        
    }
    
    if( !debug ) display.deinit();
    
	return 0;
}
