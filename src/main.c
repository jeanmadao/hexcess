#include <stdio.h>
#include <ncurses.h>
#include "../include/utils.h"
#include "../include/tui.h"

int main(int argc, char **argv) {
    FILE *fp;
    unsigned char *content;
    unsigned long content_len;
    struct win hex_win, plain_win, controls_win;
    int key;

    fp = parse_args(argc, argv);
    content_len = get_file_content(fp, &content);
    init_tui();
    init_windows(&hex_win, &plain_win, &controls_win);
    display_windows(&hex_win, &plain_win, &controls_win, content, content_len);

    while(1) {
        key = getch();
        switch(key) {
            case KEY_UP:
                break;
            case KEY_RESIZE:
                resize_windows(&hex_win, &plain_win, &controls_win);
                display_windows(&hex_win, &plain_win, &controls_win, content, content_len);
                break;
        }
    }
    fclose(fp);
    end_tui();

    return 0;
}
