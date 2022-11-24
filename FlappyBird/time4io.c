#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int getsw(void) {

    // shift bits 11-8 to the right, and with 1111 to remove all other values
    int sw = (PORTD >> 8) & 0xF;

    return sw;
}

int getbtns(void) {

    // shift bits 7-5 to the right, and with 111 to remove all other values
    int btn = (PORTD >> 5) & 0x7;

    return btn;
}

