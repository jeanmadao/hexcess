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

enum win_name {
    HEX,
    PLAIN,
};

enum dir {
    LEFT, RIGHT, UP, DOWN
};

struct nav {
    enum win_name active_win;
    int y;
    int x;
    int max_y;
    int max_x;
    unsigned char byte;
};

void init_tui();
void end_tui();
void init_windows(struct win *, struct win *, struct win *, struct nav *);
void resize_windows(struct win *, struct win *, struct win *, struct nav *);
void display_windows(struct win *, struct win *, struct win *, struct nav *,
        unsigned char *, unsigned long);
void decorate_windows(struct win *, struct win *, struct win *);
void move_cursor(struct nav *, enum dir, struct win *, struct win *, unsigned char *);
void display_cursor(struct nav *, struct win *, struct win *);
#endif
