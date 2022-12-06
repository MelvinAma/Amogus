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

int mytime = 0x5957;

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
}

/* This function is called repetitively from the main program */
// Game states: 0 = start menu, 1 = game, 2 = end screen
void gameWork(void) {

    // Clear string
    display_clear_strings();

    if (gameState == 0) {
        while (1)
        {
            /* code */
        
            display_update();
            display_image(0, canvas);

            moveBird(0, 0);
            moveBird(20, 10);
            moveBird(35, 10);
            moveBird(127, 31);
        }
    }

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
            // TODO: fix not registering BTN1
            if (getbtns() >= 1 && getbtns() <= 4) {
                countdown();
                gameState = 1;
            }
        }
    }

    // Game is live
    if (gameState == 1) {
        y += 1; // Gravity, positive since the Y-axis is inverted

        // FOR TESTING:
        // turning on the left-most switch resets the game
        if (getsw() == 8) {
            gameState = 0;
            // To not register multiple key presses, which would lead to gameState == 0 being skipped
            delay(2000);
        }
        //moveBird(x, y); TODO

        if (getbtns() == 4) {
            y -= 2;
        }
    }

    // End screen / game over
    if (gameState == 2) {
    }

    moveBird(x, y);
    //movePipes();
}


