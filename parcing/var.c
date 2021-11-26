//
// Created by fcody on 11/12/21.
//

#include "../main.h"

void	concatenation(char *src, int i, int len, t_vars *var)
{
    char *new_string;
    char *temp;

    new_string = malloc(sizeof(char) * (len + 1));
//	dprintf(2, "malloc 28 %p", new_string);
    ft_strlcpy(new_string, src + i, len + 1);
    temp = var->new;
    var->new = ft_strjoin(var->new, new_string);
//	dprintf(2, "malloc 32 %p", var->new);
    free(temp);
    free(new_string);
//     printf("result of concatenation:|%s|\n", var->new);
}

void vars_init(t_vars *vars, char **env)
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
//	dprintf(2, "malloc 49 %p", vars->new);
}

void vars_init_end(t_vars *vars, char **env)
{
	vars->q1 = 0;
	vars->q2 = 0;
	vars->line = NULL;
	vars->key = NULL;
	vars->value = NULL;
	// free(vars->var);
	vars->var = NULL;
	vars->env = env;
	vars->start_i = 0;
	vars->end_i = 0;
}

void filling_key_val(t_vars *var, char *env_str)
{
	int i = 0;
	while(env_str[i] && env_str[i] != '=')
		i++;
	var->key = malloc(sizeof(char) * i + 1);
	ft_strlcpy(var->key, env_str, i + 1);
	var->value = malloc(sizeof(char) * (ft_strlen(env_str + i + 1) + 1));
	ft_strlcpy(var->value, env_str + i + 1, ft_strlen(env_str + i + 1) + 1);
}

void find_value(t_vars *var)
{
	int i = 0;

	while(var->env[i])
	{
		filling_key_val(var, var->env[i]);
		if (!ft_strncmp(var->key, var->var, ft_strlen(var->var) + 1))
			break;
		free(var->key);
		var->key = NULL;
		free(var->value);
		var->value = NULL;
		i++;
	}
	if (!var->env[i])
		var->value = ft_strdup("");
}

int unique_value_cases(int type, t_vars *var, t_data *data)
{
	if (type == 1)
	{
		var->value = ft_strdup("$");
		return(0);
	}
	if (type == 2)
	{
		var->value = ft_strdup(ft_itoa(data->exit_status));
		return(1);
	}
	var->value = ft_strdup("42021");
	return(1);
}

int find_variable(t_vars *var, t_data *data)
{
	int i = var->end_i + 1;
	int j = 0;

	if (!var->line[i] || var->line[i] == ' ' || var->line[i] == '=' || (var->q2 && var->line[i] == '\"'))		// здесь добавить в сет все символы кроме ?$_ и алнум
		return(unique_value_cases(1, var, data));
	else if (var->line[i] == '?')
		return(unique_value_cases(2, var, data));
	else if (var->line[i] == '$')
		return(unique_value_cases(3, var, data));
	while(var->line[i] && var->line[i] != ' ' && (ft_isalnum(var->line[i]) || var->line[i] == '_'))
	{
		j++;
		if (j == 1 && ft_isdigit(var->line[i]))
			break;
		i++;
	}
	var->var = malloc(sizeof(char) * (j + 1));
	ft_strlcpy(var->var, var->line + var->end_i + 1, j + 1);
	find_value(var);
	return(j);
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

void handling_variables(t_vars *var, t_data *data)
{
	int i = 0;

	while(var->line[i])
	{
		if (var->line[i] == '\'' && !var->q2)
			var->q1 = (var->q1 + 1) % 2;
		else if (var->line[i] == '\"' && !var->q1)
			var->q2 = (var->q2 + 1) % 2;
		else
			if (var->line[i] == '$' && !var->q1)
			{
				var->end_i = i;
				concatenation(var->line, var->start_i, var->end_i - var->start_i, var);
				i += find_variable(var, data);
				concatenation(var->value, 0, ft_strlen(var->value), var);
				var->start_i = i + 1;
				fresher(var);
			}
			i++;
	}
	concatenation(var->line, var->start_i, i, var);
}

void vars(t_data *data)
{
	t_vars var;

	vars_init(&var, data->envp);
	var.line = data->line;
	handling_variables(&var, data);
	// char *temp;
	// temp = data->line;
	data->line = var.new;
	// free(temp);
	if(data->debug)
		printf(">>> handled line:|%s|\n", data->line);
	free(var.line);
	vars_init_end(&var, data->envp);
}