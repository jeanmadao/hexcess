#ifndef UTILS_H
# define UTILS_H

#include <stdio.h>

FILE *parse_args(int argc, char **argv);
unsigned char *get_file_content(FILE *fp);

#endif
