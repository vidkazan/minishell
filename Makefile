SRC = *.c builtins/*.c redirects/*.c arr_list_str_functions/*.c execution/*.c parcing/*.c

MSG = OK!

OBJ = $(SRC:.c=.o)

NAME = minishell

all: main.h
	@ gcc -lreadline $(SRC) libft/libft.a -o $(NAME)
	@ echo OK!