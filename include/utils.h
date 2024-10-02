#ifndef UTILS_H
# define UTILS_H

#include <stdio.h>

FILE *parse_args(int, char **);
unsigned long get_file_content(FILE *, unsigned char **);
void save_file(FILE *fp, unsigned char *content, unsigned long content_len);
int hex_to_byte(unsigned char *byte_hex);


#endif
