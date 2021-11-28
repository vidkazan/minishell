/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 19:54:06 by cstarmie          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/11/28 20:04:27 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

=======
/*   Updated: 2021/11/28 19:54:07 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


>>>>>>> 980feadcf526f65239c3ec757acb1254fc91a618
#include "../main.h"

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

void	handling_variables(t_vars *v, t_data *data)
{
	int	i;

	i = 0;
	while (v->line && v->line[i])
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
	t_vars	var;

	if (data->debug)
		dprintf(2, ">>> %d vars\n", getpid());
	vars_init(&var, data->envp, 1);
	var.line = data->line;
	handling_variables(&var, data);
	data->line = var.new;
	if (data->debug)
		printf(">>> handled line:|%s|\n ", data->line);
	free(var.line);
	vars_init(&var, data->envp, 0);
}
