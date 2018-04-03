#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include "CPU.h"
#include "DISPLAY.h"

const char* filename = "rom.gb";

int main( int argc, const char* argv[] ) {
    
    CPU cpu;
    DISPLAY display;

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
    
    if( argc < 2 ) {
        if( !cpu.loadROM( filename ) ) return -1;
    }
    else {
        if( !cpu.loadROM( argv[1] ) ) return -1;
    }
    
    if( !debug ) display.init();
    
    while(1) {
        cpu.run();
        if( !ACT ) return 0;
        if( !debug ) display.draw();
        
        int input = getch();
        if( input == 'q' or input == 'Q' or input == KEY_EXIT ) break;
    }
    
    if( !debug ) display.deinit();
    
	return 0;
}
