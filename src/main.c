#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

FILE *parse_args(int argc, char **argv) {
    FILE *fp;

    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Could not open file.");
        exit(1);
    }

    return fp;
}

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

unsigned char *get_file_content(FILE *fp) {
    long size;
    unsigned char byte;
    unsigned char *content;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET);
    content = (unsigned char *)malloc(size);

    if (content == NULL) {
        perror("Could not allocate memory.");
        exit(1);
    }

    fread(content, 1, size, fp);

    return content;
}

int main(int argc, char **argv) {
    FILE *fp;
    unsigned char *content;

    fp = parse_args(argc, argv);
    content = get_file_content(fp);
    init_tui();
    

    end_tui();

    return 0;
}
