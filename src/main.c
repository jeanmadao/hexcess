#include <stdio.h>
#include <ncurses.h>
#include "../include/utils.h"
#include "../include/tui.h"

int main(int argc, char **argv) {
    FILE *fp;
    unsigned char *content;
    unsigned long content_len;

    WINDOW *hex_win, *plain_win, *controls_win;
    settings sett;
    cursor cur;
    int run = 1;

    fp = parse_args(argc, argv);
    content_len = get_file_content(fp, &content);

    init_tui();
    init_windows(&hex_win, &plain_win, &controls_win, &sett, &cur);
    display_windows(hex_win, plain_win, controls_win, &sett, content, content_len, &cur);

    while (run) {

        refresh();
        wrefresh(hex_win);
        wrefresh(plain_win);
        wrefresh(controls_win);
        handle_key(hex_win, plain_win, controls_win, &cur, &sett, content, content_len, &run);

    }
    end_windows(hex_win, plain_win, controls_win);
    end_tui();
    fclose(fp);

    return 0;
}
