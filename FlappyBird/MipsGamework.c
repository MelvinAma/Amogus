/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "Declarations.h" /* Declatations for these labs */

#define gravity 1
#define start_X 20
#define start_Y 10

int mytime = 0x5957;
int bird_x = start_X;
int bird_y = start_Y;
int gameState = 0;
int gameTick = 0;

char textstring[] = "text, more text, and even more text!";

// http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf page 44 0xBF8 (SRFs special function register) + pic32mx.h for remaining

/* Interrupt Service Routine */
void user_isr(void) {
    return;
}

/* Lab-specific initialization goes here */
void gameInit(void) {
    display_init();
    display_clear_strings();
    gameState = 0;
}

/* This function is called repetitively from the main program */
// Game states: 0 = start menu, 1 = game, 2 = end screen
void gameWork(void) {

    // TODO: Clear display
    //display_clear_strings();

    // Start menu
    if (gameState == 0) {
        while (gameState == 0) {
            display_string(0, "Flappy Bird!");
            display_string(1, "Press any");
            display_string(2, "button to");
            display_string(3, "continue");
            display_update();

            display_image(96, icon);

            delay(5);

            // Check if a button is pressed to start the game
            if (buttonIsPushed()) {
                countdown();
                gameState = 1;
            }
        }
    }

    // Game is live
    if (gameState == 1) {
        while (gameState == 1) {
            gameTick++;

            // Clean canvas before drawing next frame
            resetCanvas();

            int i;
            for (i = 0; i < 128; ++i) {
                lightPixel(i, 0);
                lightPixel(i, 31);
            }

            // turning on the left-most switch resets the game
            if (getsw() == 8) {
                gameState = 0;
                // Short delay to not register multiple key presses, which can lead to gameState == 0 being skipped
                delay(1000);
            }

            if (gameTick % 5 == 0) {
                bird_y += gravity; // Gravity, positive since the Y-axis is inverted
            }
            switch (getbtns()) {
                case 4: // BTN4
                    bird_y -= 2; // Jump
                case 3: // BTN3
                    bird_x -= 1; // Move left
                case 2: // BTN2
                    bird_x += 1; // Move right
                default: // No button input
                    break;
            }

            drawBird(bird_x, bird_y);

            display_image(0, canvas);

//        if (collision()) {  // Game over
//            gameState = 2;
//        }
        }
    }

    // End screen / game over
    if (gameState == 2) {

    }
}


