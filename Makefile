SRC = *.c

MSG = OK!

OBJ = $(SRC:.c=.o)

NAME = minishell

all: main.h
	@ gcc -lreadline $(SRC) libft/libft.a -o $(NAME)
	@ echo OK!