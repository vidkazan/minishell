/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cstarmie <cstarmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 20:02:45 by cstarmie          #+#    #+#             */
/*   Updated: 2021/11/28 20:06:23 by cstarmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	vars_init(t_vars *vars, char **env, int mode)
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
	if (mode)
		vars->new = ft_strdup("");
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
