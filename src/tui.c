#include <ncurses.h>
#include <curses.h>
#include "../include/tui.h"

void init_tui() {
    initscr();
    if (has_colors()) {
        start_color();
        use_default_colors();
    }
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void end_tui() {
    nocbreak();
    keypad(stdscr, FALSE);
    echo();
    curs_set(1);
    endwin();
}

void set_windows_props(struct win *hex_win,
                       struct win *plain_win,
                       struct win *controls_win) {
    controls_win->nlines = 1;
    controls_win->ncols = COLS;
    controls_win->begin_y = LINES - controls_win->nlines;
    controls_win->begin_x = 0;

    hex_win->nlines = LINES - controls_win->nlines;
    hex_win->ncols = 53;
    hex_win->begin_y = 0;
    hex_win->begin_x = 0;

    plain_win->nlines = LINES - controls_win->nlines;
    plain_win->ncols = COLS - hex_win->ncols;
    plain_win->begin_y = 0;
    plain_win->begin_x = hex_win->ncols;
}

void decorate_windows(struct win *hex_win, struct win *plain_win,
                  struct win *controls_win) {
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_BLUE, -1);
    init_pair(3, COLOR_BLUE, COLOR_GREEN);
    wattrset(hex_win->window, COLOR_PAIR(2));
    wattrset(plain_win->window, COLOR_PAIR(1));
    wbkgd(controls_win->window, COLOR_PAIR(3));

    wborder_set(hex_win->window, 0, 0, 0, 0, 0, 0, 0, 0);
    wborder_set(plain_win->window, 0, 0, 0, 0, 0, 0, 0, 0);

    /*wrefresh(hex_win->window);*/
    /*wrefresh(plain_win->window);*/
    /*wrefresh(controls_win->window);*/
}


void init_windows(struct win *hex_win, struct win *plain_win,
                  struct win *controls_win, struct nav *navigation) {

    set_windows_props(hex_win, plain_win, controls_win);
    hex_win->window = newwin(hex_win->nlines, hex_win->ncols,
                      hex_win->begin_y, hex_win->begin_x);
    plain_win->window = newwin(plain_win->nlines, plain_win->ncols,
                      plain_win->begin_y, plain_win->begin_x);
    controls_win->window = newwin(controls_win->nlines, controls_win->ncols,
                      controls_win->begin_y, controls_win->begin_x);

    navigation->active_win = HEX;
    navigation->x = 0;
    navigation->y = 0;
    navigation->max_x = 16;
    navigation->max_y = hex_win->nlines - 2;

}

void resize_windows(struct win *hex_win, struct win *plain_win,
                  struct win *controls_win, struct nav *navigation) {

    set_windows_props(hex_win, plain_win, controls_win);

    mvwin(hex_win->window, hex_win->begin_y, hex_win->begin_x);
    mvwin(plain_win->window, plain_win->begin_y, plain_win->begin_x);
    mvwin(controls_win->window, controls_win->begin_y, controls_win->begin_x);

    wresize(hex_win->window, hex_win->nlines, hex_win->ncols);
    wresize(plain_win->window, plain_win->nlines, plain_win->ncols);
    wresize(controls_win->window, controls_win->nlines, controls_win->ncols);

    wclear(hex_win->window);
    wclear(plain_win->window);
    wclear(controls_win->window);
    navigation->max_y = hex_win->nlines - 2;


}

void display_windows(struct win *hex_win, struct win *plain_win, 
                     struct win *controls_win, struct nav *navigation,
                     unsigned char *content, unsigned long content_len) {
    const int START_BYTE_INDEX = 0;
    unsigned char byte;


    decorate_windows(hex_win, plain_win, controls_win);
    for (int i = 0; i < navigation->max_y & i * navigation->max_x < content_len ; i++) {
        mvwprintw(hex_win->window, 1 + i, 2, "%08x: ", i * navigation->max_x);
        for (int j = START_BYTE_INDEX; j < navigation->max_x & i * navigation->max_x + j < content_len; j++) {
            byte = content[i * navigation->max_x + j];
            mvwprintw(hex_win->window, 1 + i, 12 + j*2 + j/2, "%02x", byte);
            if ((byte < 33) | (byte > 126)) {
                byte = '.';
            }
            mvwprintw(plain_win->window, 1 + i, 2 + j, "%c", byte);
        }
    }
    navigation->byte = content[navigation->y * navigation->max_x + navigation->x];
}

void move_cursor(struct nav *navigation, enum dir direction, struct win *hex_win,
                 struct win *plain_win, unsigned char *content) {
    mvwprintw(hex_win->window, 1 + navigation->y, 12 + navigation->x*2 + navigation->x/2, "%02x", navigation->byte);
    if (navigation->byte < 33 | navigation->byte > 126) {
        mvwprintw(plain_win->window, 1 + navigation->y, 2 + navigation->x, ".");
    } else {
        mvwprintw(plain_win->window, 1 + navigation->y, 2 + navigation->x, "%c", navigation->byte);
    }
    switch (direction) {
        case LEFT:
            if (navigation->x > 0) {
                navigation->x--;
            }
            break;
        case RIGHT:
            if (navigation->x < navigation->max_x - 1) {
                navigation->x++;
            }
            break;
        case UP:
            if (navigation->y > 0) {
                navigation->y--;
            }
            break;
        case DOWN:
            if (navigation->y < navigation->max_y - 1) {
                navigation->y++;
            }
            break;
    }
    navigation->byte = content[navigation->y * navigation->max_x + navigation->x];
}

void display_cursor(struct nav *navigation, struct win *hex_win,
                    struct win *plain_win) {
    wattron(hex_win->window, A_STANDOUT | A_BLINK);
    mvwprintw(hex_win->window, 1 + navigation->y, 12 + navigation->x*2 + navigation->x/2, "%02x", navigation->byte);
    wattroff(hex_win->window, A_STANDOUT | A_BLINK);

    wattron(plain_win->window, A_STANDOUT);
    if (navigation->byte < 33 | navigation->byte > 126) {
        mvwprintw(plain_win->window, 1 + navigation->y, 2 + navigation->x, ".");
    } else {
        mvwprintw(plain_win->window, 1 + navigation->y, 2 + navigation->x, "%c", navigation->byte);
    }
    wattroff(plain_win->window, A_STANDOUT);
}
