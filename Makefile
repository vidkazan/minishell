SRC = *.c arr_str_lst_functions/*.c builtins/*.c execution/*.c parcing/*.c redirects/*.c main_utils/*.c

LDFLAGS="-L/usr/local/opt/readline/lib"
CPPFLAGS="-I/usr/local/opt/readline/include"

MSG = OK!

OBJ = $(SRC:.c=.o)

NAME = minishell

C_FLAGS = -Wall -Wextra -Werror

all: main.h
	@ gcc $(SRC) $(C_FLAGS) -lreadline $(LDFLAGS) $(CPPFLAGS) libft/libft.a -o $(NAME)
	@ echo OK!
