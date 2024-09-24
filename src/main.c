#include <stdio.h>
#include <ncurses.h>
#include "../include/utils.h"
#include "../include/tui.h"

int main(int argc, char **argv) {
    FILE *fp;
    unsigned char *content;
    WINDOW *current_win;
    int key;

    fp = parse_args(argc, argv);
    content = get_file_content(fp);
    init_tui();


    refresh();
    current_win = init_windows(LINES, COLS);
    while(1) {

        key = getch();
        switch(key) {
            case KEY_UP:
                printw("%d %d\n", COLS, LINES);
                break;
            case KEY_RESIZE:
                printw("resize!\n");
                break;
        }
    }
    end_tui();

    return 0;
}
