/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcody <fcody@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 08:17:02 by fcody             #+#    #+#             */
/*   Updated: 2021/11/28 08:26:05 by fcody            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <sys/wait.h>
# include <sys/signal.h>
# include <signal.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/errno.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

# define CMD 1
# define PIPE 2
# define SIMPLE_REDIRECT_INPUT 3
# define SIMPLE_REDIRECT_OUTPUT 4
# define DOUBLE_REDIRECT_INPUT 5
# define DOUBLE_REDIRECT_OUTPUT 6

typedef struct s_data	t_data;

typedef struct s_vars
{
	char	*line;
	char	*new;
	char	**env;
	char	*var;
	char	*key;
	char	*value;
	int		q1;
	int		q2;
	int		start_i;
	int		end_i;

}	t_vars;

typedef struct s_elem
{
	int				type;
	int				is_builtin;
	int				pfd[2];
	pid_t			pid;
	char			**cmd;
	char			*str;
	struct s_elem	*next;
	struct s_elem	*prev;
	t_data			*data;
}				t_elem;

typedef struct s_data
{
	char	*path;
	int		std_in;
	int		std_out;
	char	**envp;
	int		exit_status;
	t_elem	*elem_start;
	int		simple_redirect_input_fd;
	int		simple_redirect_output_fd;
	int		double_redirect_output_fd;
	int		double_redirect_input_fd;
	int		debug;
	int		exec;
	int		q1;
	int		q2;
	char	*line;
}				t_data;

typedef struct s_del_quotes_helper
{
	int	i;
	int	j;
	int	q1;
	int	q2;
	int	flag;
}				t_del_quotes_helper;

// init and close

int			data_reboot(t_data *data);
void		init(t_data *data);
void		closing(t_data *data);
void		close_fd(t_elem *elem);

// exec

void		execution_not_exec(t_elem *elem);
int			execution_fork(t_elem *elem);
void		execution_parent_builtin(t_elem *elem);
int			execution_pipe(t_elem *elem);
void		execution(t_elem *elem);
void		execution_child_last_cmd(t_elem *elem);
void		execution_child_first_cmd(t_elem *elem);
void		execution_child_first_pipe(t_elem *elem);
void		execution_child_middle_pipe(t_elem *elem);
void		execution_child(t_elem *elem);
void		waiting(t_data *data);

// finding

void		env_path_find(t_data *data);
void		find_path(t_elem *elem);
char		*search_strings_in_array(char **arr, char *search_word, \
									int *index, int mode);

// redirects

void		redirects(t_data *data);
t_elem		*double_redirect_output(t_elem	*elem);
t_elem		*simple_redirect_input(t_elem *elem);
t_elem		*simple_redirect_output(t_elem *elem);
t_elem		*double_redirect_input(t_elem *elem);

// parsing

int			main_preparser(t_data *data, char *line);
void		vars(t_data *data);
char		**shell_split(char *s, char c);
void		quotes(int i, t_data *data);

// exit status

void		execve_error(t_elem *elem, char *cmd, char *arg, char *msg);
void		exit_code_print(t_elem *elem, int write_fd);
int			error_code_transform(void);

// list functions

void		list_cleaner(t_elem *elem);
t_elem		*delete_current_node(t_data *data, t_elem	*elem);
t_elem		*create_elem(t_data *data);
t_elem		*push_back(t_elem *ptr, t_data *data);
void		print_current_elem(t_elem *ptr);
void		print_elems(t_elem *ptr);

// utils
long long	ft_atoi_overflow(const char *str);
int			ft_strlen_arr(char **arr);
char		**ft_arrjoin(char **s1, char **s2);
char		**ft_arrjoin_one_line(char **envp, char *new_arr);
int			ft_strcmp(const char *s1, const char *s2);
char		**ft_arrdup(char **env);
void		free_arr(char **str);

// builtin functions

int			builtin_fd_gen(t_elem *elem);
void		builtin_exec(t_elem *elem);
void		builtin_check(t_elem *elem);
void		builtins_error(t_data *data, char *cmd, char *arg, char *msg);
void		builtin_exit(t_elem *elem);
void		builtin_unset(t_elem *elem);
void		builtin_check(t_elem *elem);
void		builtin_echo(t_elem *elem, int write_fd);
void		builtin_cd(t_elem *elem);
void		builtin_pwd(t_elem *elem, int write_fd);
void		builtin_env(t_elem *elem, int write_fd);
void		builtin_export(t_elem *elem, int write_fd);
int			builtin_fd_gen(t_elem *elem);

// env

void		edit_env_keys(int env_index, char *new_value, t_data *data);

// export

void		export_set(t_elem *elem);
void		export_sort(t_elem *elem, int write_fd);
int			export_argument_check(char *arg);
void		edit_env_keys(int env_index, char *new_value, t_data *data);
int			is_in_export_line(char *line, char **export_arr);
int			is_min_arr_line(char *line, char **envp, char **export_arr);

#endif
