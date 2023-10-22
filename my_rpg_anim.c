/*
** EPITECH PROJECT, 2023
** my rpg
** File description:
** animation for char and map 
*/

#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow* window;
    sfEvent event;
    sfClock* clock = sfClock_create();
    sfTime elapsed;

    //  main Window
    window = sfRenderWindow_create(mode, "Top-Down Map", sfResize | sfClose, NULL);
    if (!window) return -1;

    // Load map texture
    sfTexture* mapTexture = sfTexture_createFromFile("assets/map.png", NULL);
    if (!mapTexture) return -1;

    sfSprite* mapSprite = sfSprite_create();
    sfSprite_setTexture(mapSprite, mapTexture, sfTrue);

    // Load char textures
    sfTexture* characterTextures[4][6]; // 2D array for all directions and frames
    sfSprite* characterSprite = sfSprite_create();
    int currentDirection = 0;
    int currentFrame = 0;
    sfBool isMoving = sfFalse;

    // Load character textures for all directions and frames
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

    sfVector2f characterPosition = {535.0f, 52.0f}; // Starting position

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

        // Draw the map sprite
        sfRenderWindow_drawSprite(window, mapSprite, NULL);

        // set character texture based on movement direction and frame
        sfSprite_setTexture(characterSprite, characterTextures[currentDirection][currentFrame], sfTrue);
        sfSprite_setPosition(characterSprite, characterPosition);
        sfRenderWindow_drawSprite(window, characterSprite, NULL);

        // handle user input (key)
        float movement = 0.10f;
        if (sfKeyboard_isKeyPressed(sfKeyUp)) {
            characterPosition.y -= movement;
            currentDirection = 0; // back /up
            isMoving = sfTrue;
        }
        else if (sfKeyboard_isKeyPressed(sfKeyDown)) {
            characterPosition.y += movement;
            currentDirection = 3; // down / font
            isMoving = sfTrue;
        }
        else if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
            characterPosition.x -= movement;
            currentDirection = 2; // char left
            isMoving = sfTrue;
        }
        else if (sfKeyboard_isKeyPressed(sfKeyRight)) { // char to right
            characterPosition.x += movement;
            currentDirection = 1; 
            isMoving = sfTrue;
        }
        else {
            isMoving = sfFalse;
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
