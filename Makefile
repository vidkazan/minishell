SRC = *.c arr_str_lst_functions/*.c builtins/*.c execution/*.c parcing/*.c redirects/*.c

MSG = OK!

OBJ = $(SRC:.c=.o)

NAME = minishell

all: main.h
	@ gcc -lreadline $(SRC) libft/libft.a -o $(NAME)
	@ echo OK!