#include <stdio.h>
#include <stdlib.h>

FILE *parse_args(int argc, char **argv) {
    FILE *fp;

    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "rb");
    if (!fp) {
        perror("Could not open file");
        exit(1);
    }

    return fp;
}

unsigned long save_file(unsigned char *content, unsigned long content_len,
                        char *filename) {
    FILE *fp;
    unsigned long res;
    fp = fopen(filename, "wb");
    if (!fp) {
        perror("Could not open file");
        exit(1);
    }

    res = fwrite(content, sizeof(unsigned char), content_len, fp);
    fclose(fp);

    return res;
}

int hex_to_byte(unsigned char *byte_hex, unsigned char *byte) {
    unsigned char value = 0;
    unsigned int exp = 16;
    int res = 0;

    for (unsigned int i=0; i < 2 && res != -1; i++) {
        if ((0x30 <= byte_hex[i]) && (byte_hex[i] <= 0x39)) {
            value += (byte_hex[i] - 0x30) * exp;
        } else if ((0x41 <= byte_hex[i]) && (byte_hex[i] <= 0x5a)) {
            value += (byte_hex[i] - 0x41) * exp;
        } else if ((0x61 <= byte_hex[i]) && (byte_hex[i] <= 0x7a)) {
            value += (byte_hex[i] - 0x61) * exp;
        } else {
            res = -1;
        }
        exp /= 16;
    }
    if (res == 0) {
        *byte = value;
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

