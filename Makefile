SRC = *.c

MSG = OK!

OBJ = $(SRC:.c=.o)

NAME = minishell

all: main.h
	@ gcc $(SRC) libft/libft.a -o $(NAME)
	@ echo $(MSG)