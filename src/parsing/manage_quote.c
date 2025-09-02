#include "minishell.h"

int	handle_single_quotes(char *str, char *out, int *j, int *k)
{
	(*j)++;
	while (str[*j] && str[*j] != '\'')
	{
		out[(*k)++] = str[(*j)++];
	}
	if (str[*j] == '\'')
		(*j)++;
	return (0);
}

// Copie la valeur d'une variable d'environnement dans out
static void	copy_env_value_to_out(char *env_val, char *out, int *k)
{
    int i = 0;
    while (env_val && env_val[i])
        out[(*k)++] = env_val[i++];
}

// Extrait le nom de la variable d'environnement à partir de str[*j]
static int	extract_env_name(const char *str, int *j, char *env_name)
{
    int i = 0;
    while (str[*j] && (ft_isalnum(str[*j]) || str[*j] == '_'))
        env_name[i++] = str[(*j)++];
    env_name[i] = '\0';
    return i;
}

// Gère l'expansion d'une variable d'environnement
int	handle_env_variable(t_cmd *command, char *str, char *out, int *j, int *k)
{
    char	env_name[256];
    char	*env_val;
    int		name_len;

    (*j)++;
    if (str[*j] == '?')
    {
        (*j)++;
        out[(*k)++] = '$';
        out[(*k)++] = '?';
        return 0;
    }
    name_len = extract_env_name(str, j, env_name);
    if (name_len == 0)
    {
        out[(*k)++] = '$';
        return 0;
    }
    env_val = get_env_value(command->env, env_name);
    copy_env_value_to_out(env_val, out, k);

    return 0;
}

int	handle_double_quotes(t_cmd *command, char *str, char *out, int *j, int *k)
{
	(*j)++;
	while (str[*j] && str[*j] != '\"')
	{
		if (str[*j] == '$')
			handle_env_variable(command, str, out, j, k);
		else
			out[(*k)++] = str[(*j)++];
	}
	if (str[*j] == '\"')
		(*j)++;
	return (0);
}