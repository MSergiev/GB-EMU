#ifndef INSTRUCTIONS_S
#define INSTRUCTIONS_S

#include "EXT_INSTRUCTIONS.h"
  
inline void process( BYTE inst ) {
    
    // Instruction handling
    switch( inst ) {
        case 0x00: NOP (); break;
        case 0x01: LD ( BC, WPIM() ); break;
        case 0x02: LD ( &MEM[*BC], A ); break;
        case 0x03: INC ( BC ); break;
        case 0x04: INC ( B ); break;
        case 0x05: DEC ( B ); break;
        case 0x06: LD ( B, BPIM() ); break;
        case 0x07: RLC ( A ); break;
        case 0x08: LD ( &MEM[*WPIM()], &SP ); break;
        case 0x09: ADD ( HL, BC ); break;
        case 0x0A: LD ( A, &MEM[*BC] ); break;
        case 0x0B: DEC ( BC ); break;
        case 0x0C: INC ( C ); break;
        case 0x0D: DEC ( C ); break;
        case 0x0E: LD ( C, BPIM() ); break;
        case 0x0F: RRC ( A ); break;
        
        case 0x10: STOP (); break;
        case 0x11: LD ( DE, WPIM() ); break;
        case 0x12: LD ( &MEM[*DE], A ); break;
        case 0x13: INC ( DE ); break;
        case 0x14: INC ( D ); break;
        case 0x15: DEC ( D ); break;
        case 0x16: LD ( D, BPIM() ); break;
        case 0x17: RL ( A ); break;
        case 0x18: JR ( 1, BPIM() ); break;
        case 0x19: ADD ( HL, DE ); break;
        case 0x1A: LD ( A, &MEM[*DE] ); break;
        case 0x1B: DEC ( DE ); break;
        case 0x1C: INC ( E ); break;
        case 0x1D: DEC ( E ); break;
        case 0x1E: LD ( E, BPIM() ); break;
        case 0x1F: RR ( A ); break;
        
        case 0x20: JR ( !GFZ(), BPIM() ); break;
        case 0x21: LD ( HL, WPIM() ); break;
        case 0x22: LD ( &MEM[*HL], A ); (*HL)++; break;
        case 0x23: INC ( HL ); break;
        case 0x24: INC ( H ); break;
        case 0x25: DEC ( H ); break;
        case 0x26: LD ( H, BPIM() ); break;
        case 0x27: DAA (); break;
        case 0x28: JR ( GFZ(), BPIM() ); break;
        case 0x29: ADD ( HL, HL ); break;
        case 0x2A: LD ( A, &MEM[*HL] ); (*HL)++; break;
        case 0x2B: DEC ( HL ); break;
        case 0x2C: INC ( L ); break;
        case 0x2D: DEC ( L ); break;
        case 0x2E: LD ( L, BPIM() ); break;
        case 0x2F: CPL (); break;
        
        case 0x30: JR ( !GFC(), BPIM() ); break;
        case 0x31: LD ( &SP, WPIM() ); break;
        case 0x32: LD ( &MEM[*HL], A ); (*HL)--; break;
        case 0x33: INC ( &SP ); break;
        case 0x34: INC ( &MEM[*HL] ); break;
        case 0x35: DEC ( &MEM[*HL] ); break;
        case 0x36: LD ( &MEM[*HL], BPIM() ); break;
        case 0x37: SCF (); break;
        case 0x38: JR ( GFC(), BPIM() ); break;
        case 0x39: ADD ( HL, &SP ); break;
        case 0x3A: LD ( A, &MEM[*HL] ); (*HL)--; break;
        case 0x3B: DEC ( &SP ); break;
        case 0x3C: INC ( A ); break;
        case 0x3D: DEC ( A ); break;
        case 0x3E: LD ( A, BPIM() ); break;
        case 0x3F: CCF (); break;
        
        case 0x40: LD ( B, B ); break;
        case 0x41: LD ( B, C ); break;
        case 0x42: LD ( B, D ); break;
        case 0x43: LD ( B, E ); break;
        case 0x44: LD ( B, H ); break;
        case 0x45: LD ( B, L ); break;
        case 0x46: LD ( B, &MEM[*HL] ); break;
        case 0x47: LD ( B, A ); break;
        case 0x48: LD ( C, B ); break;
        case 0x49: LD ( C, C ); break;
        case 0x4A: LD ( C, D ); break;
        case 0x4B: LD ( C, E ); break;
        case 0x4C: LD ( C, H ); break;
        case 0x4D: LD ( C, L ); break;
        case 0x4E: LD ( C, &MEM[*HL] ); break;
        case 0x4F: LD ( C, A ); break;
        
        case 0x50: LD ( D, B ); break;
        case 0x51: LD ( D, C ); break;
        case 0x52: LD ( D, D ); break;
        case 0x53: LD ( D, E ); break;
        case 0x54: LD ( D, H ); break;
        case 0x55: LD ( D, L ); break;
        case 0x56: LD ( D, &MEM[*HL] ); break;
        case 0x57: LD ( D, A ); break;
        case 0x58: LD ( E, B ); break;
        case 0x59: LD ( E, C ); break;
        case 0x5A: LD ( E, D ); break;
        case 0x5B: LD ( E, E ); break;
        case 0x5C: LD ( E, H ); break;
        case 0x5D: LD ( E, L ); break;
        case 0x5E: LD ( E, &MEM[*HL] ); break;
        case 0x5F: LD ( E, A ); break;
        
        case 0x60: LD ( H, B ); break;
        case 0x61: LD ( H, C ); break;
        case 0x62: LD ( H, D ); break;
        case 0x63: LD ( H, E ); break;
        case 0x64: LD ( H, H ); break;
        case 0x65: LD ( H, L ); break;
        case 0x66: LD ( H, &MEM[*HL] ); break;
        case 0x67: LD ( H, A ); break;
        case 0x68: LD ( L, B ); break;
        case 0x69: LD ( L, C ); break;
        case 0x6A: LD ( L, D ); break;
        case 0x6B: LD ( L, E ); break;
        case 0x6C: LD ( L, H ); break;
        case 0x6D: LD ( L, L ); break;
        case 0x6E: LD ( L, &MEM[*HL] ); break;
        case 0x6F: LD ( L, A ); break;
        
        case 0x70: LD ( &MEM[*HL], B ); break;
        case 0x71: LD ( &MEM[*HL], C ); break;
        case 0x72: LD ( &MEM[*HL], D ); break;
        case 0x73: LD ( &MEM[*HL], E ); break;
        case 0x74: LD ( &MEM[*HL], H ); break;
        case 0x75: LD ( &MEM[*HL], L ); break;
        case 0x76: HALT(); break;
        case 0x77: LD ( &MEM[*HL], A ); break;
        case 0x78: LD ( A, B ); break;
        case 0x79: LD ( A, C ); break;
        case 0x7A: LD ( A, D ); break;
        case 0x7B: LD ( A, E ); break;
        case 0x7C: LD ( A, H ); break;
        case 0x7D: LD ( A, L ); break;
        case 0x7E: LD ( &MEM[*HL], A ); break;
        case 0x7F: LD ( A, A ); break;
        
        case 0x80: ADD ( A, B ); break;
        case 0x81: ADD ( A, C ); break;
        case 0x82: ADD ( A, D ); break;
        case 0x83: ADD ( A, E ); break;
        case 0x84: ADD ( A, H ); break;
        case 0x85: ADD ( A, L ); break;
        case 0x86: ADD ( A, &MEM[*HL] ); break;
        case 0x87: ADD ( A, A ); break;
        case 0x88: ADC ( A, B ); break;
        case 0x89: ADC ( A, C ); break;
        case 0x8A: ADC ( A, D ); break;
        case 0x8B: ADC ( A, E ); break;
        case 0x8C: ADC ( A, H ); break;
        case 0x8D: ADC ( A, L ); break;
        case 0x8E: ADC ( A, &MEM[*HL] ); break;
        case 0x8F: ADC ( A, A ); break;
        
        case 0x90: SUB ( A, B ); break;
        case 0x91: SUB ( A, C ); break;
        case 0x92: SUB ( A, D ); break;
        case 0x93: SUB ( A, E ); break;
        case 0x94: SUB ( A, H ); break;
        case 0x95: SUB ( A, L ); break;
        case 0x96: SUB ( A, &MEM[*HL] ); break;
        case 0x97: SUB ( A, A ); break;
        case 0x98: SBC ( A, B ); break;
        case 0x99: SBC ( A, C ); break;
        case 0x9A: SBC ( A, D ); break;
        case 0x9B: SBC ( A, E ); break;
        case 0x9C: SBC ( A, H ); break;
        case 0x9D: SBC ( A, L ); break;
        case 0x9E: SBC ( A, &MEM[*HL] ); break;
        case 0x9F: SBC ( A, A ); break;
        
        case 0xA0: AND ( B ); break;
        case 0xA1: AND ( C ); break;
        case 0xA2: AND ( D ); break;
        case 0xA3: AND ( E ); break;
        case 0xA4: AND ( H ); break;
        case 0xA5: AND ( L ); break;
        case 0xA6: AND ( &MEM[*HL] ); break;
        case 0xA7: AND ( A ); break;
        case 0xA8: XOR ( B ); break;
        case 0xA9: XOR ( C ); break;
        case 0xAA: XOR ( D ); break;
        case 0xAB: XOR ( E ); break;
        case 0xAC: XOR ( H ); break;
        case 0xAD: XOR ( L ); break;
        case 0xAE: XOR ( &MEM[*HL] ); break;
        case 0xAF: XOR ( A ); break;
        
        case 0xB0: OR ( B ); break;
        case 0xB1: OR ( C ); break;
        case 0xB2: OR ( D ); break;
        case 0xB3: OR ( E ); break;
        case 0xB4: OR ( H ); break;
        case 0xB5: OR ( L ); break;
        case 0xB6: OR ( &MEM[*HL] ); break;
        case 0xB7: OR ( A ); break;
        case 0xB8: CP ( B ); break;
        case 0xB9: CP ( C ); break;
        case 0xBA: CP ( D ); break;
        case 0xBB: CP ( E ); break;
        case 0xBC: CP ( H ); break;
        case 0xBD: CP ( L ); break;
        case 0xBE: CP ( &MEM[*HL] ); break;
        case 0xBF: CP ( A ); break;
        
        case 0xC0: RET ( !GFZ() ); break;
        case 0xC1: POP ( BC ); break;
        case 0xC2: JP ( !GFZ(), WPIM() ); break;
        case 0xC3: JP ( 1, WPIM() ); break;
        case 0xC4: CALL ( !GFZ() ); break;
        case 0xC5: PUSH ( BC ); break;
        case 0xC6: ADD ( A, BPIM() ); break;
        case 0xC7: RST ( 0x00 ); break;
        case 0xC8: RET ( GFZ() ); break;
        case 0xC9: RET ( 1 ); break;
        case 0xCA: JP ( GFZ(), WPIM()  ); break;
        case 0xCB: processExt ( *BPIM() ); break;
        case 0xCC: CALL ( GFZ() ); break;
        case 0xCD: CALL ( 1 ); break;
        case 0xCE: ADC ( A, BPIM() ); break;
        case 0xCF: RST ( 0x08 ); break;
        
        case 0xD0: RET ( !GFC() ); break;
        case 0xD1: POP ( DE ); break;
        case 0xD2: JP ( !GFC(), WPIM() ); break;
        case 0xD3: ERR(); break;
        case 0xD4: CALL ( !GFC() ); break;
        case 0xD5: PUSH ( DE ); break;
        case 0xD6: SUB ( A, BPIM() ); break;
        case 0xD7: RST ( 0x10 ); break;
        case 0xD8: RET ( GFC() ); break;
        case 0xD9: RETI (); break;
        case 0xDA: JP ( GFC(), WPIM() ); break;
        case 0xDB: ERR(); break;
        case 0xDC: CALL ( GFC() ); break;
        case 0xDD: ERR(); break;
        case 0xDE: SBC ( A, BPIM() ); break;
        case 0xDF: RST ( 0x18 ); break;
        
        case 0xE0: LD ( &MEM[0xFF00+*BPIM()], A ); break;
        case 0xE1: POP ( HL ); break;
        case 0xE2: LD ( &MEM[0xFF00+*C], A ); break;
        case 0xE3: ERR(); break;
        case 0xE4: ERR(); break;
        case 0xE5: PUSH ( HL ); break;
        case 0xE6: AND ( BPIM() ); break;
        case 0xE7: RST ( 0x20 ); break;
        case 0xE8: ADD ( &SP, SBPIM() ); break;
        case 0xE9: JP ( 1, (WP)&MEM[*HL] ); break;
        case 0xEA: LD ( &MEM[*WPIM()], A ); break;
        case 0xEB: ERR(); break;
        case 0xEC: ERR(); break;
        case 0xED: ERR(); break;
        case 0xEE: XOR ( BPIM() ); break;
        case 0xEF: RST ( 0x28 ); break;
        
        case 0xF0: LD ( A, &MEM[0xFF00+*BPIM()] ); break;
        case 0xF1: POP ( AF ); break;
        case 0xF2: ERR(); break;
        case 0xF3: DI (); break;
        case 0xF4: ERR(); break;
        case 0xF5: PUSH ( AF ); break;
        case 0xF6: OR ( BPIM() ); break;
        case 0xF7: RST ( 0x30 ); break;
        case 0xF8: LDHL ( SBPIM() ); break;
        case 0xF9: LD ( &SP, HL ); break;
        case 0xFA: LD ( A, &MEM[*WPIM()] ); break;
        case 0xFB: EI (); break;
        case 0xFC: ERR(); break;
        case 0xFD: ERR(); break;
        case 0xFE: CP ( BPIM() ); break;
        case 0xFF: RST ( 0x38 ); break;
        
        default:  NOP ();    break;
    };
    
    PC++;
    CYCLES += INST_CYCLES[inst];
}

inline void ERR() {
    std::cerr << "-----------------------------------------------" << std::endl;
    std::cerr << "----------- UNSUPPORTED INSTRUCTION -----------" << std::endl;
    std::cerr << "-----------------------------------------------" << std::endl;
    ACT = 0;
}

#endif
