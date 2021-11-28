SRC = *.c arr_str_lst_functions/*.c builtins/*.c execution/*.c parcing/*.c redirects/*.c main_utils/*.c

MSG = OK!

OBJ = $(SRC:.c=.o)

NAME = minishell

C_FLAGS = -Wall -Wextra -Werror

all: main.h
	@ gcc $(SRC) -lreadline -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include libft/libft.a -o $(NAME)
	@ echo OK!
