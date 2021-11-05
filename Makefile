SRC = *.c

MSG = OK!

OBJ = $(SRC:.c=.o)

NAME = minishell

all: main.h
	gcc $(SRC) -lreadline libft/libft.a -o $(NAME)
	./$(NAME)