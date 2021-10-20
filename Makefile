SRC = *.c libft/*.c

MSG = OK!

OBJ = $(SRC:.c=.o)

NAME = minishell

all: main.h
	@ gcc $(SRC) -o $(NAME)
	@ echo $(MSG)