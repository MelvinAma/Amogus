#include <stdio.h>
#include <stdbool.h>

// Chipkit IOShield is 128x32 pixels
int HEIGHT = 32;
int WIDTH = 128;
bool pixelStatus[32][128];


bool checkCollision() {
    // If bird shares pixel with obstacle
    // true

    // Else
    // false
}

void setupGame() {
    // Set all pixels to false, aka not lit
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; ++j) {
            pixelStatus[i][j] = false;
        }
    }
}

/**
 *  Reference: https://learn.digilentinc.com/Documents/326
 */
void handleGraphics() {

}

int main() {
    setupGame();


    bool gameOver = false;
    bool collision = false;

    // Loop for each tick (should scale)
    while (!gameOver)

        // Move Obstacles
        // Move bird (check for inputs in both X and Y direction)
        // Check for collision
        if (checkCollision()) {
            gameOver = true;
        }
    // Update score if still alive
    // Update speed
    // Loop

}


