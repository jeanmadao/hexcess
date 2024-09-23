NAME			:= hexcess
CC				:= gcc
CFLAGS			:= -Wall -Wextra #-Werror
SRC_DIR 		:= src
BUILD_DIR 		:= build
INCLUDE_DIR 	:= include
SRCS 			:= $(shell find $(SRC_DIR) -name '*.c')
OBJS 			:= $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
INCLUDES 		:= $(shell find $(INCLUDE_DIR) -name '*.h')
LIBRARIES 		:= ncurses

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -l$(LIBRARIES) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE) $(BUILD_DIR)
	@echo "Compiling: $@"
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug:
	echo $(SRCS)
	echo $(INCLUDES)
	echo $(OBJS)

.PHONY: all clean fclean re
