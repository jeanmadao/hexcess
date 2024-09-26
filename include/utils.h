#ifndef UTILS_H
# define UTILS_H

#define ctrl(x) ((x) & 0x1f)
#include <stdio.h>

FILE *parse_args(int, char **);
unsigned long get_file_content(FILE *, unsigned char **);

#endif
