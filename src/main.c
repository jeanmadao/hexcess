#include <stdio.h>
#include <ncurses.h>
#include "../include/utils.h"
#include "../include/tui.h"

int main(int argc, char **argv) {
    FILE *fp;
    unsigned char *content;
    unsigned long content_len;
    struct win hex_win, plain_win, controls_win;
    struct nav navigation;
    int key;

    fp = parse_args(argc, argv);
    content_len = get_file_content(fp, &content);
    fclose(fp);
    init_tui();
    init_windows(&hex_win, &plain_win, &controls_win, &navigation);
    display_windows(&hex_win, &plain_win, &controls_win, &navigation, content, content_len);
    display_cursor(&navigation, &hex_win, &plain_win);

    while(1) {

        refresh();
        wrefresh(hex_win.window);
        wrefresh(plain_win.window);
        wrefresh(controls_win.window);
        key = getch();
        switch(key) {
            case 'k':
            case KEY_UP:
                move_cursor(&navigation, UP, &hex_win, &plain_win, content);
                display_cursor(&navigation, &hex_win, &plain_win);
                break;
            case 'j':
            case KEY_DOWN:
                move_cursor(&navigation, DOWN, &hex_win, &plain_win, content);
                display_cursor(&navigation, &hex_win, &plain_win);
                break;
            case 'h':
            case KEY_LEFT:
                move_cursor(&navigation, LEFT, &hex_win, &plain_win, content);
                display_cursor(&navigation, &hex_win, &plain_win);
                break;
            case 'l':
            case KEY_RIGHT:
                move_cursor(&navigation, RIGHT, &hex_win, &plain_win, content);
                display_cursor(&navigation, &hex_win, &plain_win);
                break;
            case KEY_RESIZE:
                resize_windows(&hex_win, &plain_win, &controls_win, &navigation);
                display_windows(&hex_win, &plain_win, &controls_win, &navigation, content, content_len);
                display_cursor(&navigation, &hex_win, &plain_win);
                break;
        }
    }
    end_tui();

    return 0;
}
