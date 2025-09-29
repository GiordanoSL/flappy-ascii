#include "keyboard.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/select.h>

// Struct para guardar as configurações canonicas do input do teclado
static struct termios oldt;

// Thread handle da thread do teclado
static pthread_t kb_thread;

// Ponteiro para a função que irá tratar o input do teclado
static key_handler_t handler = NULL;

static int running = 0;

// Ativa o modo raw (não espera o \n para ler os caracteres)
static void enable_raw_mode() {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

static void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}


static void* keyboard_loop(void* arg) {
    unsigned char buf[8];
    char c;

    while (running) {
        fd_set set;
        struct timeval timeout;

        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);

        timeout.tv_sec = 0;
        timeout.tv_usec = 16666; // ~16 ms

        int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);

        if (rv > 0 && FD_ISSET(STDIN_FILENO, &set)) {
            int n = read(STDIN_FILENO, buf, sizeof(buf));

            if (n > 0) {
                if (buf[0] == 27) { // ESC
                    if (n == 1) {
                        // Just ESC pressed alone
                        c = KEY_ESCAPE;
                    } else if (n >= 3 && buf[1] == '[') {
                        switch (buf[2]) {
                            case 'A': c = KEY_UP;    break;
                            case 'B': c = KEY_DOWN;  break;
                            case 'C': c = KEY_RIGHT; break;
                            case 'D': c = KEY_LEFT;  break;
                            default:
                                // Unknown escape sequence
                                continue;
                        }
                    } else {
                        // ESC followed by something else
                        continue;
                    }
                } else {
                    // Normal key (letters, numbers, etc.)
                    c = buf[0];
                }

                if (handler) handler(c);
            }
        }
        // If rv == 0: timeout expired, just loop again
    }

    return NULL;
}

void keyboard_init() {
    enable_raw_mode();
    running = 1;
    pthread_create(&kb_thread, NULL, keyboard_loop, NULL);
}

void keyboard_shutdown() {
    running = 0;
    pthread_cancel(kb_thread);    // stop the thread
    pthread_join(kb_thread, NULL);
    disable_raw_mode();
}

void keyboard_set_handler(key_handler_t h) {
    handler = h;
}
