#include <stdio.h>
#include <stdlib.h>

FILE *parse_args(int argc, char **argv) {
    FILE *fp;

    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Could not open file");
        exit(1);
    }

    return fp;
}

unsigned char *get_file_content(FILE *fp) {
    long size;
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

