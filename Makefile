NAME = hexcess

CC = gcc
CFLAGS = -Wall -Wextra #-Werror
SRC_DIR = src/
BUILD_DIR = build/
INCLUDE_DIR = includes/
FILES = main.c
SRCS = $(wildcard $(SRC_DIR)*.c)
OBJS = $(SRCS:$(SRC_DIR)%.c=$(BUILD_DIR)%.o)
INCLUDES = $(wildcard $(INCLUDE_DIR)*.h)
LIBRARIES = ncurses

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -l$(LIBRARIES) -o $@ $^

$(OBJS): $(SRCS) $(INCLUDES) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
