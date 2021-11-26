//
// Created by fcody on 11/12/21.
//

#include "../main.h"

typedef struct s_vars
{
    char *line;
    char *new;
    char **env;
    char *var;
    char *key;
    char *value;
    int q1;
    int q2;
    int start_i;
    int end_i;

}	t_vars;

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
	free(vars->var);
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
//     printf("key=%s\nval=%s\n", var->key, var->value);
}

void find_value(t_vars *var)
{
    int i = 0;

//     printf("var=%s\n", var->var);
    while(var->env[i])
    {
        filling_key_val(var, var->env[i]);
        if (!ft_strncmp(var->key, var->var, ft_strlen(var->var) + 1))
            break;
        free(var->key);
        free(var->value);
//        free(var->var);
        i++;
    }
    if (!var->env[i])
        var->value = ft_strdup("");
}

int find_variable(t_vars *var, t_data *data)
{
    int i = var->end_i + 1;
    int j = 0;

    if (!var->line[i] || var->line[i] == ' ' || var->line[i] == '=' || (var->q2 && var->line[i] == '\"'))		// здесь добавить в сет все символы кроме ?$_ и алнум
    {
        var->value = ft_strdup("$");
        return(j);
    }
	else if (var->line[i] == '?')
    {
        var->value = ft_strdup(ft_itoa(data->exit_status));
        return(++j);
    }
    else if (var->line[i] == '$')
    {
        var->value = ft_strdup("42021");
        return(++j);
    }
    while(var->line[i] && var->line[i] != ' ' && (ft_isalnum(var->line[i]) || var->line[i] == '_'))
    {
        j++;
        if (j == 1 && ft_isdigit(var->line[i]))
            break;
        i++;											// здесь добавить проверку на ис_алнум + '_', ис_альфа или '_' для первого символа
    }
    var->var = malloc(sizeof(char) * (j + 1));
//	dprintf(2, "malloc 122 %p", var->var);
    ft_strlcpy(var->var, var->line + var->end_i + 1, j + 1);
    find_value(var);

    return(j);
}

void	fresher(t_vars *var)
{
    var->key = NULL;
    var->value = NULL;
    var->var = NULL;
}

void handling_variables(t_vars *var, t_data *data)
{
    int i = 0;

    while(var->line[i])
    {
        if (var->line[i] == '\'' && !var->q2)
        {
//            printf(",");
            var->q1 = (var->q1 + 1) % 2;
        }
        else if (var->line[i] == '\"' && !var->q1)
        {
//            printf(";");
            var->q2 = (var->q2 + 1) % 2;
        }
        else
//            printf("%c", var->line[i]);
        if (var->line[i] == '$' && !var->q1)
        {
            var->end_i = i;
            concatenation(var->line, var->start_i, var->end_i - var->start_i, var);
            i += find_variable(var, data); // должна вернуть количество символов имени переменной без учета знака бакса
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
    // free(var.line);
    data->line = var.new;
    if(data->debug)
        printf(">>> handled line:|%s|\n", data->line);
//    free(var.new);
    free(var.line);
    vars_init_end(&var, data->envp);
}