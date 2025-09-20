#ifndef SCREEN_H
#define SCREEN_H

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
    BRANCO
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

// Print the current screen array to the terminal.
void printscr(void);

#endif // SCREEN_H