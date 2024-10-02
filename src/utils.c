#include <stdio.h>
#include <stdlib.h>

FILE *parse_args(int argc, char **argv) {
    FILE *fp;

    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r+");
    if (!fp) {
        perror("Could not open file");
        exit(1);
    }

    return fp;
}

void save_file(FILE *fp, unsigned char *content, unsigned long content_len) {
    fwrite(content, content_len, sizeof(unsigned char), fp);
}

int hex_to_byte(unsigned char *byte_hex, unsigned char *byte) {
    unsigned char value = 0;
    int exp = 16;
    int res = 0;
    for (unsigned int i=0; i < 2 && res != -1; i++) {
        if ((0x30 <= byte[i]) && (byte[i] <= 0x39)) {
            value += (byte[i] - 0x30) * exp;
        } else if {
        }
        exp /= 16;
    }



    return res;
}

unsigned long get_file_content(FILE *fp, unsigned char **content) {
    unsigned long size;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp); // get current file pointer
    fseek(fp, 0, SEEK_SET);
    *content = (unsigned char *)malloc(size);

    if (*content == NULL) {
        perror("Could not allocate memory.");
        exit(1);
    }

    fread(*content, 1, size, fp);

    return size;
}

