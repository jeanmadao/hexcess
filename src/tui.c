#include <ncurses.h>
#include <curses.h>
#include "../include/tui.h"
#include <locale.h>

void init_tui() {
    initscr();
    if (has_colors()) {
        setlocale(LC_ALL, "");
        start_color();
        use_default_colors();
    }
    noecho();
    raw();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void end_tui() {
    noraw();
    keypad(stdscr, FALSE);
    echo();
    curs_set(1);
    endwin();
}

void calculate_display(display *settings) {
    settings->controls_nlines = 1;
    settings->controls_ncols = COLS;
    settings->controls_begin_y = LINES - settings->controls_nlines;
    settings->controls_begin_x = 0;

    settings->hex_nlines = LINES - settings->controls_nlines - 2;
    settings->hex_ncols = 51;
    settings->hex_begin_y = 1;
    settings->hex_begin_x = 1;

    settings->plain_nlines = LINES - settings->controls_nlines - 2;
    settings->plain_ncols = settings->bytes_per_line + 2;
    settings->plain_begin_y = 1;
    settings->plain_begin_x = settings->hex_ncols + 3;
}

void init_windows(WINDOW **pt_hex_win, WINDOW **pt_plain_win,
                  WINDOW **pt_controls_win, display *settings, cursor *cur) {

    settings->bytes_per_line = 16;
    settings->top_line_index = 0;
    cur->i = 0;
    cur->j = 0;

    calculate_display(settings);

    *pt_hex_win = newwin(settings->hex_nlines, settings->hex_ncols,
                         settings->hex_begin_y, settings->hex_begin_x);
    *pt_plain_win = newwin(settings->plain_nlines, settings->plain_ncols,
                         settings->plain_begin_y, settings->plain_begin_x);
    *pt_controls_win = newwin(settings->controls_nlines, settings->controls_ncols,
                         settings->controls_begin_y, settings->controls_begin_x);

    scrollok(*pt_hex_win, 1);
    scrollok(*pt_plain_win, 1);


    color_windows(*pt_hex_win, *pt_plain_win, *pt_controls_win);
}

void end_windows(WINDOW *hex, WINDOW *plain, WINDOW *controls) {
    scrollok(hex, 0);
    delwin(hex);
    delwin(plain);
    delwin(controls);
}

void color_windows(WINDOW *hex_win, WINDOW *plain_win, WINDOW *controls_win) {
    init_pair(1, COLOR_BLUE, -1);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    wattrset(hex_win, COLOR_PAIR(1));
    wattrset(plain_win, COLOR_PAIR(2));
    wbkgd(controls_win, COLOR_PAIR(3));
}

void print_hex(WINDOW *hex_win, unsigned char byte, int i, int j, int attrs) {
    wattron(hex_win, A_BOLD | attrs);
    mvwprintw(hex_win, i, 11 + j * 2 + j/2, "%02x", byte);
    wattroff(hex_win, A_BOLD | attrs);
}

void print_plain(WINDOW *plain_win, unsigned char byte, int i, int j,
                 int attrs) {
    wattron(plain_win, A_BOLD | attrs);
    if (byte < 33 || byte > 126)
        byte = '.';
    mvwaddch(plain_win, i, 1 + j, byte);
    wattroff(plain_win, A_BOLD | attrs);
}

void print_row_index(WINDOW *hex_win, unsigned long row_index, int i,
                     int attrs) {
    wattron(hex_win, attrs);
    mvwprintw(hex_win, i, 1, "%08lx:", row_index);
    wattroff(hex_win, attrs);
}

void draw_windows_border(WINDOW *win, char *title) {
    int beg_y, beg_x, max_y, max_x;
    cchar_t tl, tr, bl, br;

    setcchar(&tl, L"\u256D", A_NORMAL, 0, NULL);
    setcchar(&tr, L"\u256E", A_NORMAL, 0, NULL);
    setcchar(&br, L"\u256F", A_NORMAL, 0, NULL);
    setcchar(&bl, L"\u2570", A_NORMAL, 0, NULL);

    getbegyx(win, beg_y, beg_x);
    getmaxyx(win, max_y, max_x);

    mvadd_wch(beg_y - 1, beg_x - 1, &tl);
    mvadd_wch(beg_y - 1, beg_x +  max_x, &tr);
    mvadd_wch(max_y + 1, beg_x - 1, &bl);
    mvadd_wch(max_y + 1, beg_x +  max_x, &br);

    mvhline_set(beg_y - 1, beg_x, WACS_HLINE, max_x);
    mvhline_set(max_y + 1, beg_x, WACS_HLINE, max_x);
    mvvline_set(beg_y, beg_x - 1, WACS_VLINE, max_y);
    mvvline_set(beg_y, beg_x +  max_x, WACS_VLINE, max_y);

    mvprintw(beg_y - 1, beg_x + 1, "%s", title);
}

void display_windows(WINDOW *hex_win, WINDOW *plain_win, WINDOW *controls_win,
                     display *settings, unsigned char *content,
                     unsigned long content_len, cursor *cur) {
    unsigned char byte;
    unsigned int i = 0;
    unsigned int j = 0;


    for (unsigned int index = settings->top_line_index;
    index < content_len && i < settings->hex_nlines; index++) {
        if (j == 0) {
            print_row_index(hex_win,settings->top_line_index + i * settings->bytes_per_line,
                            i, A_NORMAL);
        }
        byte = content[index];
        print_hex(hex_win, byte, i, j, A_NORMAL);
        print_plain(plain_win, byte, i, j, A_NORMAL);
        j++;
        if (j == settings->bytes_per_line) {
            j = 0;
            i++;
        }
    }

    wmove(controls_win, 0, 2);

    wprintw(controls_win, "Leave");
    wattron(controls_win, A_BOLD);
    wprintw(controls_win, " <q> | ");
    wattroff(controls_win, A_BOLD);

    wprintw(controls_win, "Left");
    wattron(controls_win, A_BOLD);
    wprintw(controls_win, " <h> | ");
    wattroff(controls_win, A_BOLD);

    wprintw(controls_win, "Down");
    wattron(controls_win, A_BOLD);
    wprintw(controls_win, " <j> | ");
    wattroff(controls_win, A_BOLD);

    wprintw(controls_win, "Up");
    wattron(controls_win, A_BOLD);
    wprintw(controls_win, " <k> | ");
    wattroff(controls_win, A_BOLD);

    wprintw(controls_win, "Right");
    wattron(controls_win, A_BOLD);
    wprintw(controls_win, " <l> | ");
    wattroff(controls_win, A_BOLD);

    wprintw(controls_win, "Scroll Half+");
    wattron(controls_win, A_BOLD);
    wprintw(controls_win, " <Ctrl-D> | ");
    wattroff(controls_win, A_BOLD);

    wprintw(controls_win, "Scroll Half-");
    wattron(controls_win, A_BOLD);
    wprintw(controls_win, " <Ctrl-U>");
    wattroff(controls_win, A_BOLD);

    byte = content[settings->top_line_index + cur->i * settings->bytes_per_line + cur->j];
    display_cursor(hex_win, plain_win, cur, byte);
    draw_windows_border(hex_win, "hex");
    draw_windows_border(plain_win, "plain");
}

void display_cursor(WINDOW *hex_win, WINDOW *plain_win, cursor *cur,
                    unsigned char byte) {
    print_hex(hex_win, byte, cur->i, cur->j, A_STANDOUT);
    print_plain(plain_win, byte, cur->i, cur->j, A_STANDOUT);
}

void move_cursor(WINDOW *hex_win, WINDOW *plain_win, cursor *cur,
                 display *settings, unsigned char *content,
                 unsigned long content_len, int i, int j) {

    unsigned char byte;

    if (i < 0) {
        scroll_windows(hex_win, plain_win, -1, settings, content, content_len, cur);
        i = 0;
    } else if (i >= (int)settings->hex_nlines) {
        scroll_windows(hex_win, plain_win, 1, settings, content, content_len, cur);
        i = settings->hex_nlines - 1;
    }

    if ((i >= 0) && (j >= 0) && (j < (int)settings->bytes_per_line)
        && (i < (int)settings->hex_nlines)) {
        byte = content[settings->top_line_index + cur->i * settings->bytes_per_line + cur->j];
        print_hex(hex_win, byte, cur->i, cur->j, A_NORMAL);
        print_plain(plain_win, byte, cur->i, cur->j, A_NORMAL);
        cur->i = i;
        if (settings->top_line_index + i * settings->bytes_per_line + j < content_len) {
            cur->j = j;
        } else {
            cur->j = content_len - settings->top_line_index - i * settings->bytes_per_line - 1;
        }
        byte = content[settings->top_line_index + cur->i * settings->bytes_per_line + cur->j];
        display_cursor(hex_win, plain_win, cur, byte);
    }
}

void resize_windows(WINDOW *hex_win, WINDOW *plain_win, WINDOW *controls_win,
                    display *settings, cursor *cur) {
    calculate_display(settings);

    wclear(hex_win);
    wclear(plain_win);
    wclear(controls_win);

    mvwin(hex_win, settings->hex_begin_y, settings->hex_begin_x);
    mvwin(plain_win, settings->plain_begin_y, settings->plain_begin_x);
    mvwin(controls_win, settings->controls_begin_y, settings->controls_begin_x);

    wresize(hex_win, settings->hex_nlines, settings->hex_ncols);
    wresize(plain_win, settings->plain_nlines, settings->plain_ncols);
    wresize(controls_win, settings->controls_nlines, settings->controls_ncols);

    cur->i = 0;
    cur->j = 0;
}

void scroll_windows(WINDOW *hex_win, WINDOW *plain_win, int value,
                   display *settings, unsigned char *content,
                   unsigned long content_len,  cursor *cur) {
    long target_top_line_index;
    long max_top_line_index;
    unsigned char byte;
    int i;
    int j;

    target_top_line_index = (int)settings->top_line_index + value * (int)settings->bytes_per_line;
    max_top_line_index = content_len - (settings->hex_nlines - 1) * settings->bytes_per_line - content_len%settings->bytes_per_line;
    if (target_top_line_index < 0) {
        value -= target_top_line_index / (int)settings->bytes_per_line;
        target_top_line_index = 0;
    }
    if (target_top_line_index >= max_top_line_index) {
        value -= (target_top_line_index - max_top_line_index) / (int)settings->bytes_per_line;
        target_top_line_index = max_top_line_index;
    }
    if (value != 0) {

        byte = content[settings->top_line_index + cur->i * settings->bytes_per_line + cur->j];
        print_hex(hex_win, byte, cur->i, cur->j, A_NORMAL);
        print_plain(plain_win, byte, cur->i, cur->j, A_NORMAL);

        wscrl(hex_win, value);
        wscrl(plain_win, value);
        settings->top_line_index = target_top_line_index;

        if (value < 0) {
            i = 0;
            j = 0;
            for (unsigned int index = target_top_line_index;
            index < content_len && value + i < 0 ; index++) {
                if (j == 0) {
                    print_row_index(hex_win,settings->top_line_index + i * settings->bytes_per_line,
                                    i, A_NORMAL);

                }
                byte = content[index];
                print_hex(hex_win, byte, i, j, A_NORMAL);
                print_plain(plain_win, byte, i, j, A_NORMAL);
                j++;
                if (j == (int)settings->bytes_per_line) {
                    j = 0;
                    i++;
                }
            }
        } else {
            i = settings->hex_nlines - value;
            j = 0;
            for (unsigned int index = target_top_line_index + (settings->hex_nlines - value) * settings->bytes_per_line;
            index < content_len && i < (int)settings->hex_nlines; index++) {
                if (j == 0) {
                    print_row_index(hex_win,settings->top_line_index + i * settings->bytes_per_line,
                                    i, A_NORMAL);

                }
                byte = content[index];
                print_hex(hex_win, byte, i, j, A_NORMAL);
                print_plain(plain_win, byte, i, j, A_NORMAL);
                j++;
                if (j == (int)settings->bytes_per_line) {
                    j = 0;
                    i++;
                }
            }
            if (cur->i == settings->hex_nlines - 1 && (int)cur->j > j - 1) {
                cur->j = j - 1;
            }

        }
        byte = content[settings->top_line_index + cur->i * settings->bytes_per_line + cur->j];
        display_cursor(hex_win, plain_win, cur, byte);
    }
}
