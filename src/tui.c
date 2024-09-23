#include <ncurses.h>

void init_tui() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
}

void end_tui() {
    nocbreak();
    keypad(stdscr, FALSE);
    echo();
    endwin();
}
