/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

/* Declare display-related functions from mipslabfunc.c */
#include <stdbool.h>

void display_image(int x, const uint8_t *data);

void display_init(void);

void display_string(int line, char *s);

void display_update(void);

uint8_t spi_send_recv(uint8_t data);

extern gameState;
extern bird_x;
extern bird_y;

/* Declare lab-related functions from mipslabfunc.c */
char *itoaconv(int num);

void labwork(void);

int nextprime(int inval);

void quicksleep(int cyc);

void tick(unsigned int *timep);

/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug(volatile int *const addr);

/* Declare bitmap array containing font */
extern const uint8_t const font[128 * 8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
/* Declare bitmap array containing the canvas */
extern uint8_t canvas[128 * 4]; // 128*4 since 4*8=36, which is the display's height
/* Declare text buffer for display output */
extern char textbuffer[4][16];

/* Declare functions written by students.
   Note: Since we declare these functions here,
   students must define their functions with the exact types
   specified in the laboratory instructions. */
/* Written as part of asm lab: delay, time2string */
void delay(int);

void gameInit(void);

void gameWork(void);

void countdown(void);

void display_clear_strings(void);

void drawBird(int x, int y);

void lightPixel(int x, int y);

bool collision();

void resetCanvas();


void movePipes();

void time2string(char *, int);


/* Written as part of i/o lab: getbtns, getsw, enable_interrupt */
int getbtns(void);

int getsw(void);

bool buttonIsPushed();

void enable_interrupt(void);