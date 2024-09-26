#ifndef TUI_H
# define TUI_H

#include <ncurses.h>

struct display_s {
    unsigned int hex_nlines;
    unsigned int hex_ncols;
    unsigned int hex_begin_y;
    unsigned int hex_begin_x;

    unsigned int plain_nlines;
    unsigned int plain_ncols;
    unsigned int plain_begin_y;
    unsigned int plain_begin_x;

    unsigned int controls_nlines;
    unsigned int controls_ncols;
    unsigned int controls_begin_y;
    unsigned int controls_begin_x;

    unsigned int bytes_per_line;
    unsigned long top_line_index;
}; 

typedef struct display_s display;

struct cursor_s {
    unsigned int i;
    unsigned int j;
};

typedef struct cursor_s cursor;

void init_tui();
void end_tui();
void calculate_display(display *settings);
void init_windows(WINDOW **pt_hex_win, WINDOW **pt_plain_win,
        WINDOW **pt_controls_win, display *settings, cursor *cur);
void end_windows(WINDOW *hex, WINDOW *plain, WINDOW *controls);
void color_windows(WINDOW *hex_win, WINDOW *plain_win, WINDOW *controls_win);
void print_hex(WINDOW *hex_win, unsigned char byte, int i, int j, int attrs);
void print_plain(WINDOW *plain_win, unsigned char byte, int i, int j,
        int attrs);
void print_row_index(WINDOW *hex_win, unsigned long row_index, int i,
        int attrs);
void draw_windows_border(WINDOW *win, char *title);
void display_windows(WINDOW *hex_win, WINDOW *plain_win, WINDOW *controls_win,
        display *settings, unsigned char *content,
        unsigned long content_len, cursor *cur);
void display_cursor(WINDOW *hex_win, WINDOW *plain_win, cursor *cur,
        unsigned char byte);
void move_cursor(WINDOW *hex_win, WINDOW *plain_win, cursor *cur,
        display *settings, unsigned char *content,
        unsigned long content_len, int i, int j);
void resize_windows(WINDOW *hex_win, WINDOW *plain_win, WINDOW *controls_win,
        display *settings, cursor *cur);
void scroll_windows(WINDOW *hex_win, WINDOW *plain_win, int value,
        display *settings, unsigned char *content,
        unsigned long content_len,  cursor *cur);
#endif
