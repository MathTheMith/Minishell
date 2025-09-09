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

static void copy_exit_code_to_out(int exit_code, char *out, int *k)
{
    char exit_str[12]; // Assez pour un int
    int i = 0;
    int temp = exit_code;
    
    // Cas spécial pour 0
    if (exit_code == 0)
    {
        out[(*k)++] = '0';
        return;
    }
    
    // Convertir l'entier en string (méthode simple)
    if (temp < 0)
    {
        out[(*k)++] = '-';
        temp = -temp;
    }
    
    // Extraire les chiffres
    int digit_count = 0;
    int temp_copy = temp;
    while (temp_copy > 0)
    {
        exit_str[digit_count++] = (temp_copy % 10) + '0';
        temp_copy /= 10;
    }
    
    // Copier les chiffres dans l'ordre inverse
    for (i = digit_count - 1; i >= 0; i--)
        out[(*k)++] = exit_str[i];
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
        // CHANGEMENT: copier l'exit code au lieu de "$?"
        copy_exit_code_to_out(command->data->lec_save, out, k);
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