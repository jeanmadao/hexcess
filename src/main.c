#include <stdio.h>
#include <ncurses.h>
#include "../include/utils.h"
#include "../include/tui.h"

int main(int argc, char **argv) {
    FILE *fp;
    unsigned char *content;
    unsigned long content_len;

    WINDOW *hex_win, *plain_win, *controls_win;
    display settings;
    cursor cur;
    int key;
    int run = 1;

    fp = parse_args(argc, argv);
    content_len = get_file_content(fp, &content);
    fclose(fp);

    init_tui();
    init_windows(&hex_win, &plain_win, &controls_win, &settings, &cur);
    display_windows(hex_win, plain_win, controls_win, &settings, content, content_len, &cur);

    while (run) {

        refresh();
        wrefresh(hex_win);
        wrefresh(plain_win);
        wrefresh(controls_win);
        key = getch();
        switch(key) {
            case 'k':
            case KEY_UP:
                move_cursor(hex_win, plain_win, &cur, &settings, content, content_len, cur.i - 1, cur.j);
                break;
            case 'j':
            case KEY_DOWN:
                move_cursor(hex_win, plain_win, &cur, &settings, content, content_len, cur.i + 1, cur.j);
                break;
            case 'h':
            case KEY_LEFT:
                move_cursor(hex_win, plain_win, &cur, &settings, content, content_len, cur.i, cur.j - 1);
                break;
            case 'l':
            case KEY_RIGHT:
                move_cursor(hex_win, plain_win, &cur, &settings, content, content_len, cur.i, cur.j + 1);
                break;
            case 'd':
                scroll_windows(hex_win, plain_win, 1, &settings, content, content_len, &cur);
                break;
            case 'u':
                scroll_windows(hex_win, plain_win, -1, &settings, content, content_len, &cur);
                break;
            case '0':
                move_cursor(hex_win, plain_win, &cur, &settings, content, content_len, cur.i, 0);
                break;
            case '$':
                move_cursor(hex_win, plain_win, &cur, &settings, content, content_len, cur.i, settings.bytes_per_line - 1);
                break;
            /*case 'G':*/
            /*    break;*/
            /*case 'g':*/
            /*    key = getch();*/
            /*    switch(key) {*/
            /*        case 'g':*/
            /*            break;*/
            /*    }*/
            /*    break;*/
            case KEY_RESIZE:
                resize_windows(hex_win, plain_win, controls_win, &settings, &cur);
                display_windows(hex_win, plain_win, controls_win, &settings, content, content_len, &cur);
                break;
            case 'q':
            case ctrl('c'):
                run = 0;
                break;
            case ctrl('d'):
                scroll_windows(hex_win, plain_win, settings.hex_nlines / 2, &settings, content, content_len, &cur);
                break;
            case ctrl('u'):
                scroll_windows(hex_win, plain_win, -(settings.hex_nlines / 2), &settings, content, content_len, &cur);
                break;
        }
    }
    end_windows(hex_win, plain_win, controls_win);
    end_tui();

    return 0;
}
