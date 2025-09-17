#ifndef KEYBOARD_H
#define KEYBOARD_H

/* Printable keys */ 
#define KEY_SPACE		        32      // ' '
#define KEY_EXCLAMATION 		33      // !
#define KEY_QUOTATION		    34      // "
#define KEY_NUMBER		        35      // #
#define KEY_DOLLAR		        36      // $
#define KEY_PERCENT		        37      // %
#define KEY_AMPERSAND		    38      // &
#define KEY_APOSTROPHE	    	39      // '
#define KEY_LEFT_PARENTHESIS    40      // (
#define KEY_RIGHT_PARENTHESIS	41      // )
#define KEY_ASTERISK		    42      // *
#define KEY_PLUS		        43      // +
#define KEY_COMMA		        44      // ,
#define KEY_MINUS		        45      // -
#define KEY_PERIOD		        46      // .
#define KEY_SLASH		        47      // /
#define KEY_0		            48      // 0
#define KEY_1		            49      // 1
#define KEY_2		            50      // 2
#define KEY_3		            51      // 3
#define KEY_4		            52      // 4
#define KEY_5		            53      // 5
#define KEY_6		            54      // 6
#define KEY_7		            55      // 7
#define KEY_8		            56      // 8
#define KEY_9		            57      // 9
#define KEY_COLON		        58      // :
#define KEY_SEMICOLON		    59      // ;
#define KEY_LESS		        60      // <
#define KEY_EQUAL		        61      // =
#define KEY_GREATER		        62      // >
#define KEY_QUESTION	        63      // ?
#define KEY_AT		            64      // @
#define KEY_A		            65      // A
#define KEY_B		            66      // B
#define KEY_C		            67      // C
#define KEY_D		            68      // D
#define KEY_E		            69      // E
#define KEY_F		            70      // F
#define KEY_G		            71      // G
#define KEY_H		            72      // H
#define KEY_I		            73      // I
#define KEY_J		            74      // J
#define KEY_K		            75      // K
#define KEY_L		            76      // L
#define KEY_M		            77      // M
#define KEY_N		            78      // N
#define KEY_O		            79      // O
#define KEY_P		            80      // P
#define KEY_Q		            81      // Q
#define KEY_R		            82      // R
#define KEY_S		            83      // S
#define KEY_T		            84      // T
#define KEY_U		            85      // U
#define KEY_V		            86      // V
#define KEY_W		            87      // W
#define KEY_X		            88      // X
#define KEY_Y		            89      // Y
#define KEY_Z		            90      // Z
#define KEY_LEFT_BRACKET		91      // [
#define KEY_BACKSLASH		    92      // '\'
#define KEY_RIGHT_BRACKET		93      // ]
#define KEY_CARET		        94      // ^
#define KEY_UNDERSCORE		    95      // _
#define KEY_GRAVE		        96      // `
#define KEY_a		            97      // a
#define KEY_b		            98      // b
#define KEY_c		            99      // c
#define KEY_d		            100     // d
#define KEY_e		            101     // e
#define KEY_f		            102     // f
#define KEY_g		            103     // g
#define KEY_h		            104     // h
#define KEY_i		            105     // i
#define KEY_j		            106     // j
#define KEY_k		            107     // k
#define KEY_l		            108     // l
#define KEY_m		            109     // m
#define KEY_n		            110     // n
#define KEY_o		            111     // o
#define KEY_p		            112     // p
#define KEY_q		            113     // q
#define KEY_r		            114     // r
#define KEY_s		            115     // s
#define KEY_t		            116     // t
#define KEY_u		            117     // u
#define KEY_v		            118     // v
#define KEY_w		            119     // w
#define KEY_x		            120     // x
#define KEY_y		            121     // y
#define KEY_z		            122     // z
#define KEY_LEFT_BRACE		    123     // {
#define KEY_VERTICAL_BAR		124     // |
#define KEY_RIGHT_BRACE		    125     // }
#define KEY_TILDE		        126     // ~

/* Function keys */ 
#define KEY_ESCAPE              27      // 'ESC'
#define KEY_ENTER               10      // '\n'
#define KEY_BACKSPACE           127     // 'backspace'
#define KEY_TAB                 9       // '\t'
#define KEY_RIGHT               0       // RIGHT ARROW
#define KEY_LEFT                1       // LEFT ARROW
#define KEY_DOWN                2       // DOWN ARROW
#define KEY_UP                  3       // UP ARROW

// Type for handler functions
typedef void (*key_handler_t)(char);

// Initialize keyboard handling (starts thread, puts terminal in raw mode)
void keyboard_init();

// Restore terminal and stop thread
void keyboard_shutdown();

// Register a handler (only one active at a time for simplicity)
void keyboard_set_handler(key_handler_t handler);

#endif