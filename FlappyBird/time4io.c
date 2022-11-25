#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int getsw(void) {

    int sw = (PORTD >> 8) & 0xF;

    return sw;
}

int getbtns(void) {

    int btn = (PORTD >> 5) & 0x7;

    return btn;
}

