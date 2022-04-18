#include "curses.h"
#include "../strings/strings.h"

UIVars lbzCursesUI(){
    UIVars vars;
    initscr();				// start curses mode
	vars.hasColor = initColors();// start colors mode if available
    vars.controls = NULL;
    vars.nControls = 0;
	cbreak();				// raw buffer except controls (Ctrl+C, etc.)
	keypad(stdscr, TRUE);	// gets F keys and arrow keys
	noecho();				// disable echoing
    vars.h = getmaxy(stdscr);
    vars.w = getmaxx(stdscr);
	refresh();
    return vars;
}

void addControl(UIVars* vars, Control* control){
    if(!vars || !control) return; // if null return
    int n = vars->nControls;
    size_t memSize = (n + 1) * sizeof(Control*);
    if (!n) vars->controls = malloc(memSize);
    else vars->controls = realloc(vars->controls, memSize);
    vars->controls[vars->nControls] = control;
    vars->nControls++;
}

Control* createControl(size_t x, size_t y, size_t w, size_t h, size_t boarder, int color){
    Control* control;
    control = (Control*)malloc(sizeof(Control));
    control->href = newwin(h,w,y,x);
    control->boarder = boarder;
    control->color = color;
    if(control->boarder > w/2 || control->boarder > h/2) control->boarder = 0;
    wbkgd(control->href, color);
    if (control->boarder > 0) box(control->href, 0, 0);
    wrefresh(control->href);
    return control;
}

void addTextbox(Control* control, Textbox* textbox){
    if(!control || !textbox) return;
    int n = control->fields;
    size_t memSize = (n+1)*sizeof(Textbox*);
    if(!n) control->texts = malloc(memSize);
    else control->texts = realloc(control->texts, memSize);
    control->texts[control->fields]=textbox;
    control->fields++;
}

Textbox* createTextbox(size_t x, size_t y, size_t w, size_t h, int color, JUSTIFY just, TEXTSTATE state){
    Textbox* textbox;
    textbox = (Textbox*)malloc(sizeof(Textbox));
    textbox->h = h;
    textbox->w = w;
    textbox->x = x;
    textbox->y = y;
    textbox->color = color;
    textbox->justify = just;
    textbox->state = state;
    return textbox;
}

void setText(UIVars* vars, int control, int textbox, char* text){
    if (!vars) return;
    if(!vars->controls) return;
    if(vars->nControls <= control) return;
    if(!vars->controls[control]->texts) return;
    if(vars->controls[control]->fields <= textbox) return;
    vars->controls[control]->texts[textbox]->text = text;
}

void updateControl(Control* control){
    size_t x,y,w,h;
    for (size_t i = 0; i < control->fields; i++)
    {
        size_t npages = 0;
        x = control->texts[i]->x + control->boarder;
        y = control->texts[i]->y + control->boarder;
        w = control->texts[i]->w;
        h = control->texts[i]->h;
        if (y > control->h - h) y = control->h - h;
        char*** pages = strToPages(control->texts[i]->text, 
            control->texts[i]->w, control->texts[i]->h, &npages);
        if(!pages) continue;
        char** lines = pages[0];
        float xx=0;
        wattron(control->href, control->texts[i]->color);
        for (size_t j=0; j<h; j++) {
            if(control->texts[i]->justify == MIDDLE) xx = (w - strlen(lines[j])) / 2;
            else if(control->texts[i]->justify == RIGHT) xx = w-strlen(lines[j]);
            else xx = 0;
            if(xx<0) xx=0;
            mvwprintw(control->href, y + j, x + (size_t)xx, "%s", lines[j]);
        }
        wattroff(control->href, control->texts[i]->color);
    }
    wrefresh(control->href);
}

void updateUI(UIVars* vars){
    for (size_t i = 0; i < vars->nControls; i++)
    {
        updateControl(vars->controls[i]);
    }
    refresh();
}

void destroyUI(UIVars* vars){
    for (size_t i = 0; i < vars->nControls; i++)
    {
        wborder(vars->controls[i]->href, ' ', ' ', ' ',' ',' ',' ',' ',' '); // draw ' ' boarder
    	wrefresh(vars->controls[i]->href);
	    delwin(vars->controls[i]->href);
    }
    endwin();
}

void debug(){
    def_prog_mode();		/* Save the tty modes		  */
	endwin();			/* End curses mode temporarily	  */
	printf("Reached");
    getchar();
	reset_prog_mode();		/* Return to the previous tty mode*/
	refresh();			/* Do refresh() to restore the	  */
}

void debugPause(){
    def_prog_mode();		/* Save the tty modes		  */
	endwin();			/* End curses mode temporarily	  */
    getchar();
}

void debugResume(){
    getchar();
	reset_prog_mode();		/* Return to the previous tty mode*/
	refresh();			/* Do refresh() to restore the	  */
}