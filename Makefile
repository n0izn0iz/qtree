CC=gcc
NAME=qtree_viewer
SRCS=$(wildcard srcs/*.c)
OBJS=$(patsubst srcs/%.c,objs/%.o,$(SRCS))
LD_FLAGS=$(shell sdl2-config --libs) -lm
C_FLAGS=-Wall -Wextra -Werror -g $(shell sdl2-config --cflags) -Iincludes

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LD_FLAGS)

objs/%.o: srcs/%.c
	$(CC) -c -o $@ $(C_FLAGS) $^

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
