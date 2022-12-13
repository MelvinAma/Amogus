/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

    Modified by Ville Stenström and Melvin Amandusson


   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "Declarations.h"  /* Declatations for these labs */
#include <stdbool.h>
#include <math.h>

/* Declare a helper function which is local to this file */
static void num32asc(char *s, int);

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)


#define gravity 1
#define start_X 5
#define start_Y 5
#define defaultPipeHeight 8
#define defaultPipeGap 9

// Constants used for generating pseudo-random numbers
#define LCG_A 1664525
#define LCG_C 1013904223
#define LCG_M (1 << 31)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

int pipeGap = defaultPipeGap;
int pipeNumber = 0;
int furthestPipeIndex = 12;

// Global LCG state
static unsigned long long lcg_state = 0;

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
    int i;
    for (i = cyc; i > 0; i--);
}

/* tick:
   Add 1 to time in memory, at location pointed to by parameter.
   Time is stored as 4 pairs of 2 NBCD-digits.
   1st pair (most significant byte) counts days.
   2nd pair counts hours.
   3rd pair counts minutes.
   4th pair (least significant byte) counts seconds.
   In most labs, only the 3rd and 4th pairs are used. */
void tick(unsigned int *timep) {
    /* Get current value, store locally */
    register unsigned int t = *timep;
    t += 1; /* Increment local copy */

    /* If result was not a valid BCD-coded time, adjust now */

    if ((t & 0x0000000f) >= 0x0000000a) t += 0x00000006;
    if ((t & 0x000000f0) >= 0x00000060) t += 0x000000a0;
    /* Seconds are now OK */

    if ((t & 0x00000f00) >= 0x00000a00) t += 0x00000600;
    if ((t & 0x0000f000) >= 0x00006000) t += 0x0000a000;
    /* Minutes are now OK */

    if ((t & 0x000f0000) >= 0x000a0000) t += 0x00060000;
    if ((t & 0x00ff0000) >= 0x00240000) t += 0x00dc0000;
    /* Hours are now OK */

    if ((t & 0x0f000000) >= 0x0a000000) t += 0x06000000;
    if ((t & 0xf0000000) >= 0xa0000000) t = 0;
    /* Days are now OK */

    *timep = t; /* Store new value */
}

/* display_debug
   A function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug(volatile int *const addr) {
    display_string(1, "Addr");
    display_string(2, "Data");
    num32asc(&textbuffer[1][6], (int) addr);
    num32asc(&textbuffer[2][6], *addr);
    display_update();
}

uint8_t spi_send_recv(uint8_t data) {
    while (!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while (!(SPI2STAT & 1));
    return SPI2BUF;
}

void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    quicksleep(10);
    DISPLAY_ACTIVATE_VDD;
    quicksleep(1000000);

    spi_send_recv(0xAE);
    DISPLAY_ACTIVATE_RESET;
    quicksleep(10);
    DISPLAY_DO_NOT_RESET;
    quicksleep(10);

    spi_send_recv(0x8D);
    spi_send_recv(0x14);

    spi_send_recv(0xD9);
    spi_send_recv(0xF1);

    DISPLAY_ACTIVATE_VBAT;
    quicksleep(10000000);

    spi_send_recv(0xA1);
    spi_send_recv(0xC8);

    spi_send_recv(0xDA);
    spi_send_recv(0x20);

    spi_send_recv(0xAF);
}

void display_clear_strings(void) {
    int i;
    for (i = 0; i < 4; ++i) {
        display_string(i, "");
    }
    display_update();
}

// Does nothing until a button is pressed,
// prevents activations caused by the button being held down beforehand
void waitForInput() {
    pressed = true;
    while (1) {
        if (buttonIsPushed()) {
            if (pressed == false) {
                break;
            }
        } else {
            pressed = false;
        }
    }
}

void countdown(void) {
    display_clear_strings();

    display_string(0, "BTN4: jump");
    display_string(1, "BTN3: move left");
    display_string(2, "BTN2: move right");
    display_string(3, "SW4 : reset game");

    display_update();

    waitForInput();

    display_clear_strings();
    display_string(2, "   3");
    display_update();
    delay(1000);
    display_string(2, "   2");
    display_update();
    delay(1000);
    display_string(2, "   1");
    display_update();
    delay(1000);
    display_string(2, "   GO!");
    display_update();
    delay(1000);
}

void display_string(int line, char *s) {
    int i;
    if (line < 0 || line >= 4)
        return;
    if (!s)
        return;

    for (i = 0; i < 16; i++)
        if (*s) {
            textbuffer[line][i] = *s;
            s++;
        } else
            textbuffer[line][i] = ' ';
}

bool buttonIsPushed() {
    if (getbtns() >= 1 && getbtns() <= 4) {
        return 1;
    }
    return 0;
}

// Remove all markings from the canvas
// Canvas is set to all 1's (pixels turned off)
void resetCanvas() {
    int i;
    for (i = 0; i < 512; ++i) {
        canvas[i] = 255;
    }
}

// Makes changes to canvas[] to light pixels on the screen
void lightPixel(int x, int y) {
    // Check if x and y are out of bounds for the screen
    // Check if x and y are out of bounds for the screen
    if (y < 0 || x < 0 || x > 127 || y > 32) {
        return;
    }

    // Adjust x and y based on the "block" of the screen they are in
    // "block" meaning the four different y-sections that the screen is divided into
    // Does nothing if the y equals 0-7 (first row)
    int blocks[] = {8, 16, 24};
    int i;
    for (i = 0; i < 3; i++) {

        // check which section we should target
        if (y >= blocks[i] && y < blocks[i] + 8) {

            // remove from y based on the row to get a number between 0-7
            y = y - blocks[i];

            // simulate row change by adding 128 for every additional row
            x = x + 128 * (i + 1);

        }
    }

    // Perform a bit manipulation operation on canvas[x] based on y

    // if y == 0, we want to target and change the first bit in the canvas array byte value
    if (y == 0) {
        int write = ~1;
        canvas[x] = canvas[x] & write; // bitwise and (1111 & 1110 --> 1110)
        // if not, we find it by looping through every byte, checking for correct y, then changing the canvas
    } else {
        int k = 1;
        int l;
        for (l = 1; l < 8; l++) {
            // k will be (00000010, 00000100) and so on
            k *= 2;
            if (y == l) {
                int write = ~k; // invert ex. (00000010 --> 11111101)
                canvas[x] = canvas[x] & write;
            }
        }
    }
}

// The bird is represented as a 2x2 square
// The coordinates use the top-left pixel as (x,y)
void drawBird(Bird *bird) {
    int i;
    int j;
    for (i = 0; i < bird->height; i++) {
        for (j = 0; j < bird->width; j++) {
            lightPixel(bird->x + i, bird->y + j);
        }
    }
}

void drawPipe(Pipe *pipe) {
    // Use the drawPixel function to draw each pixel of the pipe
    int i, j;
    for (i = 0; i < pipe->width; i++) {
        for (j = 0; j < pipe->height; j++) {
            lightPixel(pipe->x + i, pipe->y + j);
        }
    }
}

void drawPipes() {
    int i;
    for (i = 0; i < numPipes; i++) {
        drawPipe(&pipes[i]);
    }
}

// Set up the pipes
// The BOTTOM pipe's values are inherited from the UPPER pipe
void initPipes() {
    int i;
    int random1;
    int random2;

    for (i = 0; i < numPipes; i += 2) {
        random1 = lcg_rand();// Pseudo-random for lower pipe
        random2 = lcg_rand();// Pseudo-random for lower pipe

        // Initialize the values of the top Pipe struct
        pipes[i] = (Pipe) {
                .x = i * 22 + 64 - score,
                .y = 0,
                .width = 8,
                .height = 8 + (random1 % 5),
        };

        // Initialize the values of the bottom Pipe struct
        pipes[i + 1] = (Pipe) {
                .x = pipes[i].x,
                .y = 32 - (8 + (random2 % 5)),
                .width = 8,
                .height = 34 - pipes[i + 1].y + 10,
        };
    }
}

// Move all pipes one step to the left
// If a pipe is moved outside of the screen, it is placed to the right of the screen again
// The new position is based on the second-furthest pipe pair

// Makes changes to canvas[] to "mark" pixels on the screen
// to display the pipe's movement
void movePipes() {
    int i;
    int j;
    int shift;
    if (score > 20) { // Prevent pipes from intersecting
        shift = 20;
    } else {
        shift = score;
    }

    // Find the furthest pipe pair and its index
    furthestPipeIndex = 0;
    for (j = 0; j < numPipes; j += 2) {
        if (pipes[j].x > pipes[furthestPipeIndex].x) {
            furthestPipeIndex = j;
        }
    }

    for (i = 0; i < numPipes; i += 2) {
        // Update the pipe's position
        pipes[i].x -= x_speed;
        pipes[i + 1].x -= x_speed;

        int random1 = lcg_rand();    // Pseudo-random
        int random2 = lcg_rand();    // Pseudo-random

        if (pipes[i].x <= 0 - pipes[i].width) {
            // Initialize the values of the top Pipe struct
            pipes[i] = (Pipe) {
                    .x = pipes[furthestPipeIndex].x + 40 - shift,
                    .y = 0,
                    .width = 8,
                    .height = 8 + (random1 % 5),
            };

            // Initialize the values of the bottom Pipe struct
            pipes[i + 1] = (Pipe) {
                    .x = pipes[i].x,
                    .y = 32 - (8 + (random2 % 5)),
                    .width = 8,
                    .height = 34 - pipes[i + 1].y + 10,
            };
        }
    }
}

bool outOfBounds() {
    if (bird.y <= 0 || bird.y >= 31 - bird.height) {
        return true;
    }
    return false;
}

// If the bird is out of bounds, move it back to the screen
// (used during testing)
void handleOutOfBounds() {
    if (bird.x <= 0) {
        bird.x = 0;
    }
    if (bird.x >= 126 + bird.width) {
        bird.x = 126;
    }
}

// Bounding box collision detection
bool collision() {
    // The bird's hitbox is 2x2, with (0,0) being the top-left pixel
    int i;
    for (i = 0; i < numPipes; ++i) {
        if (bird.x < pipes[i].x + pipes[i].width &&
            bird.x + bird.width > pipes[i].x &&
            bird.y < pipes[i].y + pipes[i].height &&
            bird.height + bird.y > pipes[i].y) {
            return 1;
        }
    }
    return 0;
}

// Restore values
void resetGame() {
    bird.x = start_X;
    bird.y = start_Y;
    initials[0] = 'A';
    initials[1] = 'A';
    x_speed = 1;
    score = 0;
    resetCanvas();
}

void swapInt(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Names stored in pairs of chars (0-1,2-3,4-5)
void swapPlayers(int a, int b) {
    char temp = topPlayers[a * 2 + 1];
    topPlayers[a * 2 + 1] = topPlayers[b * 2 + 1];
    topPlayers[b * 2 + 1] = temp;

    temp = topPlayers[a * 2];
    topPlayers[a * 2] = topPlayers[b * 2];
    topPlayers[b * 2] = temp;
}

// Regular quicksort that sorts in ascending order
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        // Choose pivot and partition the array
        int pivot = arr[high];
        int i = low - 1;
        int j;
        for (j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                swapInt(&arr[i], &arr[j]);
                swapPlayers(i, j);
            }
        }
        swapInt(&arr[i + 1], &arr[high]);
        swapPlayers(i + 1, high);

        int pivotIndex = i + 1;

        // Recursively sort the subarrays
        quicksort(arr, low, pivotIndex - 1);
        quicksort(arr, pivotIndex + 1, high);
    }
}

// Utilizes the pre-implemented itoaconv function to
// concatenate the initials with the score
void displayHighScores() {
    display_clear_strings();
    display_string(0, "HIGH SCORES:");
    int line = 1;
    int index;
    for (index = 2; index >= 0; --index) {
        char str1[20] = {topPlayers[index * 2], topPlayers[index * 2 + 1], ' '};
        char str2[20];
        int i;
        int j;

        // call itoaconv to convert topScores[index] to a string and store the result in str2
        char *itoaResult = itoaconv(topScores[index]);
        for (i = 0; itoaResult[i] != '\0'; i++) {
            str2[i] = itoaResult[i];
        }
        str2[i] = '\0';
        // concatenate str1 and str2 and store the result in str1
        for (i = 0; str1[i] != '\0'; i++) {
            // do nothing
        }
        for (j = 0; str2[j] != '\0'; j++) {
            str1[i + j] = str2[j];
        }
        str1[i + j] = '\0';

        // Only display if a score is saved for the position
        if (topScores[index] != 0) {
            display_string(line, str1);
        }
        line++;
    }
}

// Generates a pseudo-random number using the LCG algorithm.
// Did not manage to get a proper random generator to work, so resorted to this
// https://github.com/Zielon/PBRVulkan/blob/master/PBRVulkan/RayTracer/src/Assets/Shaders/Common/Random.glsl
int lcg_rand() {
    // lcg_state is initialized to 0
    // after this, every subsequent call will use the
    // new lcg_state to calculate the next number
    lcg_state = TMR2;
    lcg_state = (LCG_A * lcg_state + LCG_C) % LCG_M;
    return (int) (lcg_state >> 1);
}

void drawDeathAnimation(Bird *bird) {
    int x = bird->x;
    int y = bird->y;

    lightPixel(x - 5, y - 5);
    lightPixel(x - 4, y - 5);
    lightPixel(x - 4, y - 4);
    lightPixel(x - 3, y - 4);
    lightPixel(x - 3, y - 3);
    lightPixel(x - 2, y - 3);
    lightPixel(x - 2, y - 2);

    lightPixel(x + 5, y + 5);
    lightPixel(x + 4, y + 5);
    lightPixel(x + 4, y + 4);
    lightPixel(x + 3, y + 4);
    lightPixel(x + 3, y + 3);
    lightPixel(x + 2, y + 3);
    lightPixel(x + 2, y + 2);

    lightPixel(x + 5, y - 5);
    lightPixel(x + 4, y - 5);
    lightPixel(x + 4, y - 4);
    lightPixel(x + 3, y - 4);
    lightPixel(x + 3, y - 3);
    lightPixel(x + 2, y - 3);
    lightPixel(x + 2, y - 2);

    lightPixel(x - 5, y + 5);
    lightPixel(x - 4, y + 5);
    lightPixel(x - 4, y + 4);
    lightPixel(x - 3, y + 4);
    lightPixel(x - 3, y + 3);
    lightPixel(x - 2, y + 3);
    lightPixel(x - 2, y + 2);
}

void display_image(int x, const uint8_t *data) {
    int i, j;

    for (i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;

        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(x & 0xF);
        spi_send_recv(0x10 | ((x >> 4) & 0xF));

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (j = 0; j < 32 * 4; j++)
            spi_send_recv(~data[i * 32 * 4 + j]);
    }
}

void display_update(void) {
    int i, j, k;
    int c;
    for (i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0x0);
        spi_send_recv(0x10);

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (j = 0; j < 16; j++) {
            c = textbuffer[i][j];
            if (c & 0x80)
                continue;

            for (k = 0; k < 8; k++)
                spi_send_recv(font[c * 8 + k]);
        }
    }
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc(char *s, int n) {
    int i;
    for (i = 28; i >= 0; i -= 4)
        *s++ = "0123456789ABCDEF"[(n >> i) & 15];
}

/*
 * nextprime
 * 
 * Return the first prime number larger than the integer
 * given as a parameter. The integer must be positive.
 */
#define PRIME_FALSE   0     /* Constant to help readability. */
#define PRIME_TRUE    1     /* Constant to help readability. */

int nextprime(int inval) {
    register int perhapsprime = 0; /* Holds a tentative prime while we check it. */
    register int testfactor; /* Holds various factors for which we test perhapsprime. */
    register int found;      /* Flag, false until we find a prime. */

    if (inval < 3)          /* Initial sanity check of parameter. */
    {
        if (inval <= 0) return (1);  /* Return 1 for zero or negative input. */
        if (inval == 1) return (2);  /* Easy special case. */
        if (inval == 2) return (3);  /* Easy special case. */
    } else {
        /* Testing an even number for primeness is pointless, since
         * all even numbers are divisible by 2. Therefore, we make sure
         * that perhapsprime is larger than the parameter, and odd. */
        perhapsprime = (inval + 1) | 1;
    }
    /* While prime not found, loop. */
    for (found = PRIME_FALSE; found != PRIME_TRUE; perhapsprime += 2) {
        /* Check factors from 3 up to perhapsprime/2. */
        for (testfactor = 3; testfactor <= (perhapsprime >> 1) + 1; testfactor += 1) {
            found = PRIME_TRUE;      /* Assume we will find a prime. */
            if ((perhapsprime % testfactor) == 0) /* If testfactor divides perhapsprime... */
            {
                found = PRIME_FALSE;   /* ...then, perhapsprime was non-prime. */
                goto check_next_prime; /* Break the inner loop, go test a new perhapsprime. */
            }
        }
        check_next_prime:;         /* This label is used to break the inner loop. */
        if (found == PRIME_TRUE)  /* If the loop ended normally, we found a prime. */
        {
            return (perhapsprime);  /* Return the prime we found. */
        }
    }
    return (perhapsprime);      /* When the loop ends, perhapsprime is a real prime. */
}

/*
 * itoa
 * 
 * Simple conversion routine
 * Converts binary to decimal numbers
 * Returns pointer to (static) char array
 * 
 * The integer argument is converted to a string
 * of digits representing the integer in decimal format.
 * The integer is considered signed, and a minus-sign
 * precedes the string of digits if the number is
 * negative.
 * 
 * This routine will return a varying number of digits, from
 * one digit (for integers in the range 0 through 9) and up to
 * 10 digits and a leading minus-sign (for the largest negative
 * 32-bit integers).
 * 
 * If the integer has the special value
 * 100000...0 (that's 31 zeros), the number cannot be
 * negated. We check for this, and treat this as a special case.
 * If the integer has any other value, the sign is saved separately.
 * 
 * If the integer is negative, it is then converted to
 * its positive counterpart. We then use the positive
 * absolute value for conversion.
 * 
 * Conversion produces the least-significant digits first,
 * which is the reverse of the order in which we wish to
 * print the digits. We therefore store all digits in a buffer,
 * in ASCII form.
 * 
 * To avoid a separate step for reversing the contents of the buffer,
 * the buffer is initialized with an end-of-string marker at the
 * very end of the buffer. The digits produced by conversion are then
 * stored right-to-left in the buffer: starting with the position
 * immediately before the end-of-string marker and proceeding towards
 * the beginning of the buffer.
 * 
 * For this to work, the buffer size must of course be big enough
 * to hold the decimal representation of the largest possible integer,
 * and the minus sign, and the trailing end-of-string marker.
 * The value 24 for ITOA_BUFSIZ was selected to allow conversion of
 * 64-bit quantities; however, the size of an int on your current compiler
 * may not allow this straight away.
 */
#define ITOA_BUFSIZ ( 24 )

char *itoaconv(int num) {
    register int i, sign;
    static char itoa_buffer[ITOA_BUFSIZ];
    static const char maxneg[] = "-2147483648";

    itoa_buffer[ITOA_BUFSIZ - 1] = 0;   /* Insert the end-of-string marker. */
    sign = num;                           /* Save sign. */
    if (num < 0 && num - 1 > 0)          /* Check for most negative integer */
    {
        for (i = 0; i < sizeof(maxneg); i += 1)
            itoa_buffer[i + 1] = maxneg[i];
        i = 0;
    } else {
        if (num < 0) num = -num;           /* Make number positive. */
        i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
        do {
            itoa_buffer[i] = num % 10 + '0';/* Insert next digit. */
            num = num / 10;                   /* Remove digit from number. */
            i -= 1;                           /* Move index to next empty position. */
        } while (num > 0);
        if (sign < 0) {
            itoa_buffer[i] = '-';
            i -= 1;
        }
    }
    /* Since the loop always sets the index i to the next empty position,
     * we must add 1 in order to return a pointer to the first occupied position. */
    return (&itoa_buffer[i + 1]);
}