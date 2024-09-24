#ifndef TUI_H
# define TUI_H

#include <ncurses.h>

void init_tui();
void end_tui();
WINDOW *init_windows(int height, int width);

#endif
