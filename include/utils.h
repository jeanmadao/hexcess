#ifndef UTILS_H
# define UTILS_H

#include <stdio.h>

FILE *parse_args(int, char **);
unsigned long get_file_content(FILE *, unsigned char **);
unsigned long save_file(unsigned char *content, unsigned long content_len,
        char *filename);
int hex_to_byte(unsigned char *byte_hex, unsigned char *byte);


#endif
