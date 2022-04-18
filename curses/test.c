#include "curses.h"
#include <stdlib.h>
#include <stdio.h>

void testUI();

int main(){
    //testStr();
    testNetwork();
    return 0;
}

void testUI(){
    UIVars ui = lbzCursesUI();
    Control* frame = createControl(0,0,ui.w,ui.h,1,COLOR_PAIR(C_BLACK_CYAN));
    Textbox* frametitle = createTextbox(20,0,20,2,COLOR_PAIR(C_BLUE_BLACK),MIDDLE,NORMAL);
    char frametitletext[] = "Helloooooooo\nWorld!";
    frametitle->text = frametitletext;
    addTextbox(frame, frametitle);
    addControl(&ui, frame);
    //debug();
    updateUI(&ui);
    getchar();
    destroyUI(&ui);
}