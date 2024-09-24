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

WINDOW *init_windows(int lines, int cols) {
    WINDOW *hex_win, *plain_win, *controls_win;
    int hex_win_lines, hex_win_cols;
    int plain_win_lines, plain_win_cols;
    int controls_win_lines, controls_win_cols;

    controls_win_lines = 5;
    controls_win_cols = cols;
    hex_win_lines = lines - controls_win_lines;
    hex_win_cols = 51;
    plain_win_lines = lines - controls_win_lines;
    plain_win_cols = cols - hex_win_cols;

    hex_win = newwin(hex_win_lines, hex_win_cols, 0, 0);
    plain_win = newwin(plain_win_lines, plain_win_cols, 0, hex_win_cols);
    controls_win = newwin(controls_win_lines, controls_win_cols, lines - controls_win_lines, 0);

    box(hex_win, 0, 0);
    box(plain_win, 0, 0);
    box(controls_win, 0,0);
    wrefresh(hex_win);
    wrefresh(plain_win);
    wrefresh(controls_win);
    return hex_win;
}
