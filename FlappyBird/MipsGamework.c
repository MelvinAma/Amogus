/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   Modified by Ville Stenström and Melvin Amandusson
   */


#include <stdint.h>  /* Declarations of uint_32 and the like */
#include "pic32mx.h"  /* Declarations of system-specific addresses etc */
#include "Declarations.h" /* Declatations for these labs */

#define gravity 1
#define start_X 5
#define start_Y 5

int mytime = 0x5957;
int gameState = 0;
int gameTick = 0;
char alphabet[27];
char initials[] = {'A', 'A'};
char topPlayers[] = {' ', ' ', ' ', ' ', ' ', ' '};
int topScores[] = {0, 0, 0};
int score = 0;
int numPipes = 12;

bool pressed = false;

Pipe pipes[12];

Bird bird = {
        .x = start_X,
        .y = start_Y,
        .width = 2,
        .height = 2
};

char textstring[] = "text, more text, and even more text!";

// http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf page 44 0xBF8 (SRFs special function register) + pic32mx.h for remaining

/* Interrupt Service Routine */
void user_isr(void) {
    return;
}

/* Lab-specific initialization goes here */
void gameInit(void) {
    int i;
    for (i = 0; i < 26; i++) {
        alphabet[i] = 'A' + i;
    }
    resetGame();
    gameState = 0;
    display_init();
    display_clear_strings();
}
/* This function is called repetitively from the main program */
// Game states: 0 = start menu, 1 = game, 2 = end screen
void gameWork(void) {
    display_clear_strings();
    resetCanvas();

    // Start menu
    if (gameState == 0) {
        while (gameState == 0) {
            display_string(0, "Flappy Bird!");
            display_string(1, "Press any");
            display_string(2, "button to");
            display_string(3, "continue");
            display_update();

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
        initPipes();

        // Freeze-frame for the first second of round start
        resetCanvas();
        int k;
        for (k = 0; k < 128; ++k) {
            lightPixel(k, 0);
            lightPixel(k, 31);
        }
        drawBird(&bird);
        movePipes();
        drawPipes();
        display_image(0, canvas);
        delay(1000);

        // MAIN GAME LOOP
        // Loops until game over
        while (gameState == 1) {
            gameTick++;

            // Clean canvas before drawing next frame
            resetCanvas();

            // Light top- and bottom border
            int j;
            for (j = 0; j < 128; ++j) {
                lightPixel(j, 0);
                lightPixel(j, 31);
            }

            // turning on the left-most switch resets the game
            if (getsw() == 8) {
                resetGame();
                gameState = 0;
                // Short delay to not register multiple key presses, which can lead to gameState == 0 being skipped
                delay(1000);
            }

            if (gameTick % 5 == 0) {
                bird.y += gravity; // Gravity, positive since the Y-axis is inverted
                movePipes();
            }

            int btn = getbtns();
            // Check the value of the getbtns function
            if (btn == 0x4) {
                // Left button is pressed
                if (!pressed) {
                    bird.y -= 10;
                }
                pressed = true;
            } else if (btn == 0x2 && !pressed) {
                // Middle button is pressed
                bird.x -= 1;
                pressed = true;
            } else if (btn == 0x1 && !pressed) {
                // Right button is pressed
                bird.x += 1;
                pressed = true;
            } else {
                // No button is pressed
                pressed = false;
            }

            int l;
            for (l = 0; l < numPipes; l += 2) {
                // Increase score everytime the bird passes a pipe
                if (bird.x == pipes[l].x + pipes[l].width) {
                    score++;
                    break;
                }
            }

            drawBird(&bird);
            drawPipes();

            display_image(0, canvas);

            handleOutOfBounds();
            gameTick++;

            if (collision() || outOfBounds()) {  // Game over
                drawDeathAnimation(&bird);
                display_image(0, canvas);
                delay(3000);
                gameState = 2;
            }
        }
    }

    // End screen / game over
    if (gameState == 2) {

        // Check if the score is enough to get into the top 3 list
        // if true, allow player to enter their initials
        if (score > topScores[0]) {
            display_clear_strings();
            display_string(0, "Final Score: ");
            display_string(2, "Name:");

            pressed = false;
            int initialIndex = 0;
            int i = 0;  // Alphabet index

            // Allow user to set initials
            // INPUTS:
            // BTN4: step forward (e.g. A -> B, Z -> A)
            // BTN3: step backwards (e.g. A <- B, Z <- A)
            // BTN2: confirms the current initial, ends loop if pressed thrice
            while (initialIndex < 2) {
                int btn = getbtns();
                if (btn == 0x4) {
                    // Left button is pressed, step FORWARD in the alphabet
                    if (!pressed) {
                        i++;
                        if (i >= 26) {   // Past Z, jump to A
                            i = 0;
                        }
                        initials[initialIndex] = alphabet[i];
                    }
                    pressed = true;

                } else if (btn == 0x2) {
                    // Middle button is pressed, step BACKWARDS in the alphabet
                    if (!pressed) {
                        i--;
                        if (i < 0) {    // Past A, jump to Z
                            i = 25;
                        }
                        initials[initialIndex] = alphabet[i];
                    }
                    pressed = true;
                } else if (btn == 0x1) {    // An initial has been confirmed
                    if (!pressed) {
                        initialIndex++;
                    }
                    pressed = true;
                } else {
                    // No button is pressed
                    pressed = false;
                }
                display_string(3, initials);
                display_update();
            }
            // Replace the third score and then sort the scores
            topPlayers[0] = initials[0];
            topPlayers[1] = initials[1];
            topScores[0] = score;
            quicksort(topScores, 0, 2);
        } else {
            display_clear_strings();
            display_string(0, "You did not");
            display_string(1, "make it into");
            display_string(2, "the top 3 :(");
            display_update();

            waitForInput();
        }

        displayHighScores();
        display_update();

        waitForInput();

        resetGame();
        gameState = 1;
    }
}


