#ifndef SCREEN_H
#define SCREEN_H

/*
---------------- SCREEN MODULE ----------------

    This module simulates a text-based screen using two layers of data.
The first is a 2D screen_buffer of size SCR_WIDTH x SCR_HEIGHT,
where each cell stores a character (the texture) along with a foreground
and background color. The coordinate system starts at (0,0) in the top-left corner.

    The second layer is a linear char array called screen, which represents
the full screen as a string ready to be printed to the terminal. Each pixel
in the buffer expands to ANSI escape codes for colors plus its texture
character, followed by a newline at the end of each row. The string is
null-terminated so it can be printed directly.

    The typical workflow is: write pixels into screen_buffer (e.g., with set_pixel),
then call update_screen() to convert it into the linear array, and finally
use printscr() to render it in the terminal.
*/

#include <stdbool.h>

#define SCR_WIDTH 48
#define SCR_HEIGHT 50

typedef enum _color{
    PRETO = 0,
    VERMELHO,
    VERDE,
    AMARELO,
    AZUL,
    MAGENTA,
    CIANO,
    CINZA_CLARO,
    CINZA,
    VERMELHO_CLARO,
    VERDE_CLARO,
    AMARELO_CLARO,
    AZUL_CLARO,
    MAGENTA_CLARO,
    CIANO_CLARO,
    BRANCO,
    CLEAR,
} Color;

typedef struct _pixel
{
    char texture;
    Color fore_color;
    Color back_color;
}Pixel;

// Initialize the screen and allocate memory for the screen array and buffer.
// Returns:
//   0  - Initialization successful
//   1  - A screen instance is already running
//  -1  - Memory allocation failed
int screen_init(void);

// Free memory used by the screen array and reset terminal state.
// Returns:
//   0  - Deletion successful
//   1  - No screen instance is running
int screen_delete(void);

// Set the foreground and background colors used when clearing the screen.
void set_clear_color(Color fore, Color back);

// Clear the screen buffer using the currently set clear colors.
// Note: This updates the buffer only; the terminal is not updated.
void clear_screen_buffer(void);

// Copy the screen buffer into the linear screen array.
// Note: This does not print to the terminal.
void update_screen(void);

// Set a pixel at (pos_x, pos_y) on the screen_buffer.
// Note: This does not update the linear screen array.
void set_pixel(int pos_x, int pos_y, char texture, Color fore, Color back);

// Draw a filled rectangle on the screen buffer.
// Top-left corner at (x, y), size given by width and height.
// Note: This only modifies the screen buffer.
void draw_rect(int pos_x, int pos_y, int width, int height, char texture, Color fore, Color back, bool border);

// Draw a rectangle outline on the screen buffer.
void draw_rect_outline(int pos_x, int pos_y, int width, int height, char texture, Color fore, Color back);

// Print the current screen array to the terminal.
void printscr(void);

#endif // SCREEN_H