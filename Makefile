NAME = hexcess

CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC_DIR = src/
BUILD_DIR = build/
INCLUDE_DIR = includes/
FILES = main.c
SRCS = $(wildcard $(SRC_DIR)*.c)
OBJS = $(SRCS:$(SRC_DIR)%.c=$(BUILD_DIR)%.o)
INCLUDES = $(wildcard $(INCLUDE_DIR)*.h)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS): $(SRCS) $(INCLUDES)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
