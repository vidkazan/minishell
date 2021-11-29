NAME = minishell

SRCS_FILES =	arr_str_lst_functions/arr_functions.c \
				arr_str_lst_functions/delete_current_node.c \
				arr_str_lst_functions/list_functions.c \
				arr_str_lst_functions/str_functions.c \
				builtins/builtins_echo.c \
				builtins/builtins_env.c \
				builtins/builtins_export_main.c \
				builtins/builtins_export_set.c \
				builtins/builtins_export_sorting.c \
				builtins/builtins_main.c \
				builtins/builtins_pwd_cd.c \
				execution/closing.c \
				execution/error.c \
				execution/execution.c \
				execution/execution_child.c \
				execution/exit_status.c \
				execution/path_finding.c \
				execution/waiting.c \
				main_utils/init.c \
				main_utils/signals.c \
				main_utils/main.c \
				parcing/pipe_and_cmd.c \
				parcing/preparsing.c \
				parcing/quotes.c \
				parcing/redirectes.c \
				parcing/shell_split.c \
				parcing/var.c \
				parcing/var_utils.c \
				redirects/redirects.c \
				redirects/redirects_di.c \
				redirects/redirects_do.c \
				redirects/redirects_si.c \
				redirects/redirects_so.c 

SRCS		= 	$(SRCS_FILES)

OBJS		=	$(SRCS:.c=.o)

READLINE = -lreadline -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include

INCLUDE		=	-I./include -I./libs/libft/

HEADER	=	include/main.h libft/libft.h

FLAGS		=	-Wall -Wextra -Werror

LIB = libft/libft.a


all:		$(NAME)

%.o:%.c HEADER
			gcc $(FLAGS) $(INCLUDE) -c $< -o $@

$(NAME):	$(OBJS)
			gcc $(OBJS) $(LIB) $(READLINE) -o $(NAME)

clean:
			rm -rf $(OBJS)

fclean:		clean
			rm -rf $(NAME)
			rm -rf msh_tester-master/$(NAME)

re:			fclean all

.PHONY:		all clean fclean re bonus