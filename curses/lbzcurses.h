#include <ncurses.h>
#include <stdlib.h>

#if !defined(LBZCURSES_MACROS)
#define LBZCURSES_MACROS

#if !defined(COLOR_MACROS)
#define COLOR_MACROS
    #define C_BLACK_BLACK       1
    #define C_BLUE_BLACK        2
    #define C_CYAN_BLACK        3
    #define C_GREEN_BLACK       4
    #define C_MAGENTA_BLACK     5
    #define C_RED_BLACK         6
    #define C_WHITE_BLACK       7
    #define C_YELLOW_BLACK      8
    #define C_BLACK_BLUE        9
    #define C_BLUE_BLUE         10
    #define C_CYAN_BLUE         11
    #define C_GREEN_BLUE        12
    #define C_MAGENTA_BLUE      13
    #define C_RED_BLUE          14
    #define C_WHITE_BLUE        15
    #define C_YELLOW_BLUE       16
    #define C_BLACK_CYAN        17
    #define C_BLUE_CYAN         18
    #define C_CYAN_CYAN         19
    #define C_GREEN_CYAN        20
    #define C_MAGENTA_CYAN      21
    #define C_RED_CYAN          22
    #define C_WHITE_CYAN        23
    #define C_YELLOW_CYAN       24
    #define C_BLACK_GREEN       25
    #define C_BLUE_GREEN        26
    #define C_CYAN_GREEN        27
    #define C_GREEN_GREEN       28
    #define C_MAGENTA_GREEN     29
    #define C_RED_GREEN         30
    #define C_WHITE_GREEN       31
    #define C_YELLOW_GREEN      32
    #define C_BLACK_MAGENTA     33
    #define C_BLUE_MAGENTA      34
    #define C_CYAN_MAGENTA      35
    #define C_GREEN_MAGENTA     36
    #define C_MAGENTA_MAGENTA   37
    #define C_RED_MAGENTA       38
    #define C_WHITE_MAGENTA     39
    #define C_YELLOW_MAGENTA    40
    #define C_BLACK_RED         41
    #define C_BLUE_RED          42
    #define C_CYAN_RED          43
    #define C_GREEN_RED         44
    #define C_MAGENTA_RED       45
    #define C_RED_RED           46
    #define C_WHITE_RED         47
    #define C_YELLOW_RED        48
    #define C_BLACK_WHITE       49
    #define C_BLUE_WHITE        50
    #define C_CYAN_WHITE        51
    #define C_GREEN_WHITE       52
    #define C_MAGENTA_WHITE     53
    #define C_RED_WHITE         54
    #define C_WHITE_WHITE       55
    #define C_YELLOW_WHITE      56
    #define C_BLACK_YELLOW      57
    #define C_BLUE_YELLOW       58
    #define C_CYAN_YELLOW       59
    #define C_GREEN_YELLOW      60
    #define C_MAGENTA_YELLOW    61
    #define C_RED_YELLOW        62
    #define C_WHITE_YELLOW      63
    #define C_YELLOW_YELLOW     64
#endif // COLOR_MACROS

typedef enum textState_t{
    NORMAL,
    ACTIVE,
    DISABLED
} TEXTSTATE;

typedef enum justify_t{
    LEFT,
    MIDDLE,
    RIGHT
} JUSTIFY;

typedef struct textbox_t{
    size_t x;
    size_t y;
    size_t w;
    size_t h;
    char* text;
    int color;
    TEXTSTATE state;
    JUSTIFY justify;
} Textbox;

typedef struct control_t{
    WINDOW* href;
    size_t y;
    size_t x;
    size_t w;
    size_t h;
    size_t boarder;
    int fields;
    int color;
    Textbox** texts;
} Control;

typedef struct uiVars_t{
    bool hasColor;
    size_t w;
    size_t h;
    Control** controls;
    int nControls;
} UIVars;

/*init UI*/
UIVars lbzCursesUI();
/*init Colors*/
bool initColors();
/*add control*/
void addControl(UIVars* vars, Control* control);
/*create control*/
Control* createControl(size_t x, size_t y, size_t w, size_t h, size_t boarder, int color);
/*add textbox*/
void addTextbox(Control* control, Textbox* textbox);
/*create textbox*/
Textbox* createTextbox(size_t x, size_t y, size_t w, size_t h, int color, JUSTIFY just, TEXTSTATE state);
/*set text*/
void setText(UIVars* vars, int control, int textbox, char* text);
/*update control*/
void updateControl(Control* control);
/*update UI*/
void updateUI(UIVars* vars);
/*destroy UI*/
void destroyUI(UIVars* vars);
/*debug*/
void debug();
/*debug pause*/
void debugPause();
/*debug resume*/
void debugResume();

#endif // LBZCURSES_MACROS