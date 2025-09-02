/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 08:11:30 by marvin            #+#    #+#             */
/*   Updated: 2025/06/10 08:11:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Remplace les variables $VAR dans line par leur valeur dans env
char *expand_variables(const char *line, t_list *env)
{
    size_t len = strlen(line);
    char *result = malloc(len + 1);
    if (!result)
        return NULL;
    size_t ri = 0;
    for (size_t i = 0; i < len; )
    {
        if (line[i] == '$' && (isalpha(line[i+1]) || line[i+1] == '_'))
        {
            size_t var_start = i + 1;
            size_t var_len = 0;
            while (isalnum(line[var_start + var_len]) || line[var_start + var_len] == '_')
                var_len++;
            char var_name[128];
            if (var_len > 0 && var_len < sizeof(var_name))
            {
                strncpy(var_name, line + var_start, var_len);
                var_name[var_len] = '\0';
                char *val = get_env_value(env, var_name);
                if (val)
                {
                    size_t vlen = strlen(val);
                    char *new_result = realloc(result, ri + vlen + (len - i - var_len) + 1);
                    if (!new_result)
                    {
                        free(result);
                        return NULL;
                    }
                    result = new_result;
                    strcpy(result + ri, val);
                    ri += vlen;
                }
                i = var_start + var_len;
                continue;
            }
        }
        result[ri++] = line[i++];
    }
    result[ri] = '\0';
    return result;
}


int bad_redirection(char **args)
{
	int i = 0;
	int redirect_count;
	int lr_count;
	int j;
	char *str;

	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0 ||
			ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
		{
			if (!args[i + 1] || args[i + 1][0] == '\0')
			{
				write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
				return -1;
			}
			if (ft_strcmp(args[i + 1], ">") == 0 || ft_strcmp(args[i + 1], ">>") == 0 ||
				ft_strcmp(args[i + 1], "<") == 0 || ft_strcmp(args[i + 1], "<<") == 0)
			{
				write(2, "minishell: syntax error near unexpected token `", 47);
				write(2, args[i + 1], ft_strlen(args[i + 1]));
				write(2, "'\n", 2);
				return -1;
			}
			i += 2;
		}
		else
		{
			redirect_count = 0;
			lr_count = 0;
			str = args[i];
			j = 0;

			while (str[j])
			{
				if (str[j] == '>')
				{
					redirect_count++;
					lr_count++;
				}
				else if (str[j] == '<')
				{
					redirect_count--;
					lr_count--;
				}
				j++;
			}

			if (redirect_count >= 3 || redirect_count <= -3)
			{
				write(2, "minishell: syntax error near unexpected token `", 47);
				if (lr_count >= 0)
				{
					if (redirect_count == 3)
						write(2, ">", 1);
					else
						write(2, ">>", 2);
				}
				else
				{
					if (redirect_count == -3)
						write(2, "<", 1);
					else
						write(2, "<<", 2);
				}
				write(2, "'\n", 2);
				return -1;
			}
			i++;
		}
	}
	return 0;
}

int handle_redirections(t_cmd *cmd)
{
    int i = 0;
    int fd;

    if (!cmd || !cmd->args)
        return 0;

    
    if (bad_redirection(cmd->args) < 0)
    {
        cmd->last_exit_code = 2;
        return -1;
    }
    // Vérifie d'abord les erreurs de syntaxe générales
    int heredoc_found = 0;
    int heredoc_pipefd[2] = {-1, -1};
    char *heredoc_buffer = NULL;
    size_t heredoc_bufsize = 0;

    while (cmd->args[i])
    {
        if (ft_strcmp(cmd->args[i], ">") == 0) // redirection sortie
        {
            if (!cmd->args[i + 1])
            {
                ft_putstr_fd("minishell: syntax error near unexpected token '>'\n", 2);
                cmd->last_exit_code = 2;
                return -1;
            }
            fd = open(cmd->args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror(cmd->args[i + 1]);
                cmd->last_exit_code = 1;
                return -1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            i += 2;
        }
        else if (ft_strcmp(cmd->args[i], ">>") == 0) // append sortie
        {
            if (!cmd->args[i + 1])
            {
                ft_putstr_fd("minishell: syntax error near unexpected token '>>'\n", 2);
                cmd->last_exit_code = 2;
                return -1;
            }
            fd = open(cmd->args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror(cmd->args[i + 1]);
                cmd->last_exit_code = 1;
                return -1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            i += 2;
        }
        else if (ft_strcmp(cmd->args[i], "<") == 0) // redirection entrée
        {
            if (!cmd->args[i + 1])
            {
                ft_putstr_fd("minishell: syntax error near unexpected token '<'\n", 2);
                cmd->last_exit_code = 2;
                return -1;
            }
            fd = open(cmd->args[i + 1], O_RDONLY);
            if (fd < 0)
            {
                ft_putstr_fd(cmd->args[i + 1], 2);
                ft_putstr_fd(": No such file or directory\n", 2);
                cmd->last_exit_code = 1;
                return -1;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
            i += 2;
        }
        else if (ft_strcmp(cmd->args[i], "<<") == 0) // heredoc
        {
            char *delimiter = cmd->args[i + 1];
            char *line = NULL;
            size_t len = 0;
            ssize_t nread;
            int quoted = 0;

            // Détecte si le délimiteur est entre quotes
            if ((delimiter[0] == '\'' && delimiter[strlen(delimiter)-1] == '\'') ||
                (delimiter[0] == '"' && delimiter[strlen(delimiter)-1] == '"'))
                quoted = 1;

            heredoc_found = 1;

            // Gestion du signal SIGINT (Ctrl+C)
            void (*old_sigint)(int) = signal(SIGINT, SIG_DFL);

            while (1)
            {
                write(1, "> ", 2);
                nread = getline(&line, &len, stdin);
                if (nread == -1)
                {
                    free(line);
                    free(heredoc_buffer);
                    signal(SIGINT, old_sigint);
                    cmd->last_exit_code = 130;
                    return -1;
                }
                if (line[nread - 1] == '\n')
                    line[nread - 1] = '\0';
                if (ft_strcmp(line, delimiter) == 0)
                    break;

                // Expansion des variables si le délimiteur n'est pas entre quotes
                char *expanded = NULL;
                if (!quoted)
                {
                    expanded = expand_variables(line, cmd->env); // à implémenter si non existant
                }
                else
                {
                    expanded = strdup(line);
                }
                if (!expanded)
                {
                    free(line);
                    free(heredoc_buffer);
                    signal(SIGINT, old_sigint);
                    cmd->last_exit_code = 1;
                    return -1;
                }

                size_t oldsize = heredoc_bufsize;
                heredoc_bufsize += strlen(expanded) + 1; // +1 pour le '\n'
                heredoc_buffer = realloc(heredoc_buffer, heredoc_bufsize + 1); // +1 pour le '\0' final
                if (!heredoc_buffer)
                {
                    free(line);
                    free(expanded);
                    signal(SIGINT, old_sigint);
                    cmd->last_exit_code = 1;
                    return -1;
                }
                memcpy(heredoc_buffer + oldsize, expanded, strlen(expanded));
                heredoc_buffer[oldsize + strlen(expanded)] = '\n';
                heredoc_buffer[heredoc_bufsize] = '\0';
                free(expanded);
            }
            signal(SIGINT, old_sigint);
            free(line);
            i += 2;
        }
        else
            i++;
    }

    // Si au moins un heredoc a été trouvé, crée le pipe et écrit le buffer
    if (heredoc_found)
    {
        if (pipe(heredoc_pipefd) == -1)
        {
            perror("minishell");
            free(heredoc_buffer);
            cmd->last_exit_code = 1;
            return -1;
        }
        if (heredoc_buffer)
        {
            write(heredoc_pipefd[1], heredoc_buffer, heredoc_bufsize);
        }
        free(heredoc_buffer);
        close(heredoc_pipefd[1]);
        dup2(heredoc_pipefd[0], STDIN_FILENO);
        close(heredoc_pipefd[0]);
    }
    return 0;
}


// echo hi >./outfiles/outfile01 | echo bye

