#ifndef TUI_H
# define TUI_H

#include <ncurses.h>

struct win {
    WINDOW *window;
    int nlines;
    int ncols;
    int begin_y;
    int begin_x;
};

struct nav {

};

void init_tui();
void end_tui();
void init_windows(struct win *, struct win *, struct win *);
void resize_windows(struct win *, struct win *, struct win *);
void display_windows(struct win *, struct win *, struct win *,
        unsigned char *, unsigned long);
#endif
