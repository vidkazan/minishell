//
// Created by fcody on 11/12/21.
//

#include "../main.h"

void	concatenation(char *src, int i, int len, t_vars *var)
{
	char	*new_string;
	char	*temp;

	new_string = malloc(sizeof(char) * (len + 1));
	ft_strlcpy(new_string, src + i, len + 1);
	temp = var->new;
	var->new = ft_strjoin(var->new, new_string);
	free(temp);
	free(new_string);
}

void	vars_init(t_vars *vars, char **env)
{
	vars->q1 = 0;
	vars->q2 = 0;
	vars->line = NULL;
	vars->key = NULL;
	vars->value = NULL;
	vars->var = NULL;
	vars->env = env;
	vars->start_i = 0;
	vars->end_i = 0;
	vars->new = ft_strdup("");
}

void	vars_init_end(t_vars *vars, char **env)
{
	vars->q1 = 0;
	vars->q2 = 0;
	vars->line = NULL;
	vars->key = NULL;
	vars->value = NULL;
	vars->var = NULL;
	vars->env = env;
	vars->start_i = 0;
	vars->end_i = 0;
}

void	filling_key_val(t_vars *var, char *env_str)
{
	int	i;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	var->key = malloc(sizeof(char) * i + 1);
	ft_strlcpy(var->key, env_str, i + 1);
	var->value = malloc(sizeof(char) * (ft_strlen(env_str + i + 1) + 1));
	ft_strlcpy(var->value, env_str + i + 1, ft_strlen(env_str + i + 1) + 1);
}

void	find_value(t_vars *var)
{
	int	i;

	i = 0;
	while (var->env[i])
	{
		filling_key_val(var, var->env[i]);
		if (!ft_strncmp(var->key, var->var, ft_strlen(var->var) + 1))
			break ;
		free(var->key);
		var->key = NULL;
		free(var->value);
		var->value = NULL;
		i++;
	}
	if (!var->env[i])
		var->value = ft_strdup("");
}

int	unique_value_cases(int type, t_vars *var, t_data *data)
{
	if (type == 1)
	{
		var->value = ft_strdup("$");
		return (0);
	}
	if (type == 2)
	{
		var->value = ft_strdup(ft_itoa(data->exit_status));
		return (1);
	}
	var->value = ft_strdup("42021");
	return (1);
}

int	find_variable(t_vars *var, t_data *data)
{
	int	i;
	int	j;

	i = var->end_i + 1;
	j = 0;
	if (!var->line[i] || var->line[i] == ' ' \
		|| var->line[i] == '=' || (var->q2 && var->line[i] == '\"'))
		return (unique_value_cases(1, var, data));
	else if (var->line[i] == '?')
		return (unique_value_cases(2, var, data));
	else if (var->line[i] == '$')
		return (unique_value_cases(3, var, data));
	while (var->line[i] && var->line[i] != ' ' \
		&& (ft_isalnum(var->line[i]) || var->line[i] == '_'))
	{
		j++;
		if (j == 1 && ft_isdigit(var->line[i]))
			break ;
		i++;
	}
	var->var = malloc(sizeof(char) * (j + 1));
	ft_strlcpy(var->var, var->line + var->end_i + 1, j + 1);
	find_value(var);
	return (j);
}

void	fresher(t_vars *var)
{
	if (var->key)
		free(var->key);
	var->key = NULL;
	free(var->value);
	var->value = NULL;
	free(var->var);
	var->var = NULL;
}

void	handling_variables(t_vars *v, t_data *data)
{
	int	i;

	i = 0;
	while (v->line[i])
	{
		if (v->line[i] == '\'' && !v->q2)
			v->q1 = (v->q1 + 1) % 2;
		else if (v->line[i] == '\"' && !v->q1)
			v->q2 = (v->q2 + 1) % 2;
		else if (v->line[i] == '$' && !v->q1)
		{
			v->end_i = i;
			concatenation(v->line, v->start_i, v->end_i - v->start_i, v);
			i += find_variable(v, data);
			concatenation(v->value, 0, ft_strlen(v->value), v);
			v->start_i = i + 1;
			fresher(v);
		}
		i++;
	}
	concatenation(v->line, v->start_i, i, v);
}

void	vars(t_data *data)
{
	if (data->debug)
		dprintf(2, ">>> %d vars\n", getpid());
	t_vars	var;

	vars_init(&var, data->envp);
	var.line = data->line;
	handling_variables(&var, data);
	data->line = var.new;
	if (data->debug)
		printf(">>> handled line:|%s|\n", data->line);
	free(var.line);
	vars_init_end(&var, data->envp);
}
