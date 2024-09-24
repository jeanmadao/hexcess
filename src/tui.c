#include <ncurses.h>
#include <curses.h>
#include "../include/tui.h"

void init_tui() {
    initscr();
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
    controls_win->nlines = 5;
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

void init_windows(struct win *hex_win, struct win *plain_win,
                  struct win *controls_win) {

    set_windows_props(hex_win, plain_win, controls_win);
    hex_win->window = newwin(hex_win->nlines, hex_win->ncols,
                      hex_win->begin_y, hex_win->begin_x);
    plain_win->window = newwin(plain_win->nlines, plain_win->ncols,
                      plain_win->begin_y, plain_win->begin_x);
    controls_win->window = newwin(controls_win->nlines, controls_win->ncols,
                      controls_win->begin_y, controls_win->begin_x);

    /*box(hex_win->window, 0, 0);*/
    wborder_set(hex_win->window, 0, 0, 0, 0, "\u256d", 0, 0, 0);
    box(plain_win->window, 0, 0);
    box(controls_win->window, 0,0);

    refresh();
    wrefresh(hex_win->window);
    wrefresh(plain_win->window);
    wrefresh(controls_win->window);
}

void resize_windows(struct win *hex_win, struct win *plain_win,
                  struct win *controls_win) {

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

    box(hex_win->window, 0, 0);
    box(plain_win->window, 0, 0);
    box(controls_win->window, 0, 0);

    /*wborder(hex_win->window, '│', '│', chtype, chtype, chtype, chtype, chtype, chtype)*/
    wborder(hex_win->window, '|', '|', '-', '-', '+', '+', '+', '+');

    refresh();
    wrefresh(hex_win->window);
    wrefresh(plain_win->window);
    wrefresh(controls_win->window);
}

void display_windows(struct win *hex_win, struct win *plain_win, 
                     struct win *controls_win, unsigned char *content,
                     unsigned long content_len) {
    const int BYTES_PER_LINE = 16;
    const int DISPLAY_LINES = hex_win->nlines - 2;
    const int START_BYTE_INDEX = 0;
    unsigned char byte;

    for (int i = 0; i < DISPLAY_LINES; i++) {
        mvwprintw(hex_win->window, 1 + i, 2, "%08x: ", i * BYTES_PER_LINE);
        for (int j = START_BYTE_INDEX; j < BYTES_PER_LINE; j++) {
            byte = content[i * BYTES_PER_LINE +j];
            mvwprintw(hex_win->window, 1 + i, 12 + j*2 + j/2, "%02x", byte);
            if ((byte < 21) | (byte > 126)) {
                byte = '.';
            }
            mvwprintw(plain_win->window, 1 + i, 2 + j, "%c", byte);
        }
    }

    wrefresh(hex_win->window);
    wrefresh(plain_win->window);
    refresh();
}

