/*
** EPITECH PROJECT, 2023
** my_rpg
** File description:
** map + collisions + animated mov
*/

#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main() {
    sfVideoMode mode = {975, 650, 32};
    sfRenderWindow* window;
    sfEvent event;
    sfClock* clock = sfClock_create();
    sfTime elapsed;

    // create the main Window
    window = sfRenderWindow_create(mode, "Top-Down Map", sfResize | sfClose, NULL);
    if (!window) return -1;

    // Map texture
    sfTexture* mapTexture = sfTexture_createFromFile("assets/mapcollisions.png", NULL);
    if (!mapTexture) return -1;

    sfSprite* mapSprite = sfSprite_create();
    sfSprite_setTexture(mapSprite, mapTexture, sfTrue);

    // Load character textures
    sfTexture* characterTextures[4][6]; // 2D array for all directions and frames
    sfSprite* characterSprite = sfSprite_create();
    int currentDirection = 0;
    int currentFrame = 0;
    sfBool isMoving = sfFalse;

    // Load character textures for all dir & frames
    char directionNames[4][15] = {
        "characterback",
        "characterright",
        "characterleft",
        "characterfront"
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            char filename[30];
            sprintf(filename, "assets/%s%02d.png", directionNames[i], j);
            characterTextures[i][j] = sfTexture_createFromFile(filename, NULL);
        }
    }
    // Load collision map
    sfImage* collisionImage = sfImage_createFromFile("assets/mapcollisions.png");
    if (!collisionImage) return -1;

    unsigned int mapWidth = 950;
    unsigned int mapHeight = 650;
    sfColor collisionColor = sfBlack;

    sfVector2f characterPosition = {535.0f, 52.0f}; // Starting position
    sfVector2f characterSize = {(float)sfImage_getSize(characterTextures[0][0]).x, (float)sfImage_getSize(characterTextures[0][0]).y};

    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        elapsed = sfClock_getElapsedTime(clock);
        if (sfTime_asMilliseconds(elapsed) > 100 && isMoving) {
            currentFrame = (currentFrame + 1) % 6; // Cycle through frames
            sfClock_restart(clock);
        }

        sfRenderWindow_clear(window, sfBlack); // Clear the window

        // Draw map sprite
        sfRenderWindow_drawSprite(window, mapSprite, NULL);

        // Set character texture based on movement direction & frame
        sfSprite_setTexture(characterSprite, characterTextures[currentDirection][currentFrame], sfTrue);
        sfSprite_setPosition(characterSprite, characterPosition);
        sfRenderWindow_drawSprite(window, characterSprite, NULL);

        // Handle user input (key)
        float movement = 0.10f;
        sfVector2f nextPosition = characterPosition; // Initialize next position to curr position

        if (sfKeyboard_isKeyPressed(sfKeyUp)) {
            nextPosition.y -= movement;
            currentDirection = 0; // char to face back / when up
            isMoving = sfTrue;
        }
        else if (sfKeyboard_isKeyPressed(sfKeyDown)) {
            nextPosition.y += movement;
            currentDirection = 3; // Set character to face front / (down)
            isMoving = sfTrue;
        }
        else if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
            nextPosition.x -= movement;
            currentDirection = 2; // Set char to left
            isMoving = sfTrue;
        }
        else if (sfKeyboard_isKeyPressed(sfKeyRight)) {
            nextPosition.x += movement;
            currentDirection = 1; // Set character to face right
            isMoving = sfTrue;
        }
        else {
            isMoving = sfFalse;
        }

        // Check for collisions with the collision map
        if (nextPosition.x >= 0 && nextPosition.x < mapWidth && nextPosition.y >= 0 && nextPosition.y < mapHeight) {
            sfColor pixelColor = sfImage_getPixel(collisionImage, (unsigned int)nextPosition.x, (unsigned int)nextPosition.y);

            if (pixelColor.r == collisionColor.r && pixelColor.g == collisionColor.g && pixelColor.b == collisionColor.b) {
                // Handle collision with black pixels 
                // Here = stop mov
                isMoving = sfFalse;
            }
            else {
                // If no collision, update character position
                characterPosition = nextPosition;
            }
        }

        // Display the window
        sfRenderWindow_display(window);
    }

    // Clean up resources
    sfTexture_destroy(mapTexture);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            sfTexture_destroy(characterTextures[i][j]);
        }
    }
    sfSprite_destroy(mapSprite);
    sfSprite_destroy(characterSprite);
    sfClock_destroy(clock);
    sfRenderWindow_destroy(window);

    return 0;
}
