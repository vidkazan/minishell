SRC = *.c arr_str_lst_functions/*.c builtins/*.c execution/*.c parcing/*.c redirects/*.c

MSG = OK!

OBJ = $(SRC:.c=.o)

NAME = minishell

all: main.h
	@ gcc $(SRC) -Wall -Wextra -Werror -lreadline -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include libft/libft.a -o $(NAME)
	@ echo OK!