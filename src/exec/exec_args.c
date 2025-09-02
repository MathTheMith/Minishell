/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvachon <mvachon@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 19:18:05 by marvin            #+#    #+#             */
/*   Updated: 2025/09/02 11:06:23 by mvachon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_all_cmd(t_cmd *cmds)
{
	char	*joined;
	char	*tmp;
	char	*new_join;
	int		i;

	if (!cmds->args || !cmds->args[0])
		return (NULL);
	joined = ft_strdup(cmds->args[0]);
	if (!joined)
		return (NULL);
	i = 1;
	while (cmds->args[i])
	{
		tmp = ft_strjoin(joined, " ");
		free(joined);
		if (!tmp)
			return (NULL);
		new_join = ft_strjoin(tmp, cmds->args[i]);
		free(tmp);
		if (!new_join)
			return (NULL);
		joined = new_join;
		i++;
	}
	return (joined);
}

int is_builtin(t_cmd *cmds)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);
	
	if (ft_strcmp(cmds->args[0], "exit") == 0
		|| ft_strcmp(cmds->args[0], "cd") == 0
		|| ft_strcmp(cmds->args[0], "echo") == 0
		|| ft_strcmp(cmds->args[0], "export") == 0
		|| ft_strcmp(cmds->args[0], "unset") == 0
		|| ft_strcmp(cmds->args[0], "env") == 0
		|| ft_strcmp(cmds->args[0], "pwd") == 0)
		return (1);
	return (0);
}
void exec_builtin(t_cmd *cmds)
{
    if (!cmds || !cmds->args || !cmds->args[0])
        return;
    if (ft_strcmp(cmds->args[0], "exit") == 0)
    {
        if (cmds->pipe_in)
            exit_input_pipeline(cmds);
        else
            exit_input(cmds);
    }
    else if (ft_strcmp(cmds->args[0], "cd") == 0)
        cd_input(cmds);
    else if (ft_strcmp(cmds->args[0], "echo") == 0)
        echo_input(cmds);
    else if (ft_strcmp(cmds->args[0], "export") == 0)
        export_input(cmds, &(cmds->env));
    else if (ft_strcmp(cmds->args[0], "unset") == 0)
        unset_input(cmds, &cmds->env);
    else if (ft_strcmp(cmds->args[0], "env") == 0)
        env_input(cmds);
    else if (ft_strcmp(cmds->args[0], "pwd") == 0)
        pwd_input(cmds);
}

int is_path_command(const char *cmd)
{
    if (!cmd)
        return (0);
    if (cmd[0] == '/')
        return (1);
    if (cmd[0] == '.' && cmd[1] == '/')
        return (1);
    if (ft_strchr(cmd, '/'))
        return (1);
    return (0);
}

void handle_path_error(t_cmd *cmds, char *cmd_name)
{
    struct stat st;
    
    if (stat(cmd_name, &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
            cmds->last_exit_code = 126;
        else if (!(st.st_mode & S_IXUSR))
            cmds->last_exit_code = 126;
        else
        {
            cmds->last_exit_code = 126;
            ft_putstr_fd(cmd_name, 2);
            ft_putstr_fd(": cannot execute binary file\n", 2);
        }
    }
    else
    {
        ft_putstr_fd(cmd_name, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        cmds->last_exit_code = 127;
    }
}

int check_args(t_cmd *cmds, char **envp)
{
    t_cmd_exec *cmd;
    t_cmd *current = cmds;
    int stdin_backup;
    int stdout_backup;

    while (current)
    {
        if (!current->args || !current->args[0])
        {
            current = current->next;
            continue;
        }

        stdin_backup = dup(STDIN_FILENO);
        stdout_backup = dup(STDOUT_FILENO);

        if (ft_strcmp(current->args[0], "..") == 0)
        {
            if (handle_redirections(current) < 0)
                cmds->last_exit_code = 2;
            else
            {
                ft_putstr_fd(current->args[0], 2);
                ft_putstr_fd(": command not found\n", 2);
                cmds->last_exit_code = 127;
            }
            current = current->next;
            continue;
        }
        int ret = handle_redirections(current);
        if (ret != 0)
        {
            dup2(stdin_backup, STDIN_FILENO);
            dup2(stdout_backup, STDOUT_FILENO);
            close(stdin_backup);
            close(stdout_backup);
            current = current->next;
            continue;
        }

        if (is_builtin(current))
        {
            exec_builtin(current);
            dup2(stdin_backup, STDIN_FILENO);
            dup2(stdout_backup, STDOUT_FILENO);
            close(stdin_backup);
            close(stdout_backup);
            current = current->next;
            continue;
        }

        cmd = init_cmd_exec(cmds, current->args[0], envp);
        if (!cmd)
        {
            if (is_path_command(current->args[0]))
                handle_path_error(current, current->args[0]);
            else
            {
				if (handle_redirections(current) < 0)
				{
					ft_putstr_fd(current->args[0], 2);
					ft_putstr_fd(": command not found\n", 2);
					current->last_exit_code = 127;
				}
            }
            current = current->next;
            continue;
        }

        execute_cmd(cmd, envp, current->args, current);
        free(cmd);

        dup2(stdin_backup, STDIN_FILENO);
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdin_backup);
        close(stdout_backup);

        current = current->next;
    }
    return 1;
}

t_cmd_exec	*init_cmd_exec(t_cmd *cmds, char *raw_cmd, char **envp)
{
	t_cmd_exec	*cmd;

	cmd = malloc(sizeof(t_cmd_exec));
	if (!cmd)
		return (NULL);

	cmd->raw_cmd = raw_cmd;
	cmd->args = NULL;
	cmd->path = find_path(cmds, raw_cmd, envp);
	if (!cmd->path)
	{
		free(cmd);
		ft_putstr_fd(cmds->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cmds->last_exit_code = 127;
		return (NULL);
	}
	return (cmd);
}
char **remove_redirections(char **args)
{
    int i = 0, j = 0;
    int count = 0;

    while (args[i])
    {
        if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0 ||
             ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
            && args[i + 1])
        {
            i += 2;
        }
        else if (args[i][0] == '\0')
        {
            i++;
        }
        else
        {
            count++;
            i++;
        }
    }

    char **new_args = malloc(sizeof(char *) * (count + 1));
    if (!new_args)
        return NULL;

    i = 0;
    j = 0;
    while (args[i])
    {
        if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0 ||
             ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
            && args[i + 1])
        {
            i += 2;
        }
        else if (args[i][0] == '\0')
        {
            i++;
        }
        else
        {
            new_args[j] = ft_strdup(args[i]);
            if (!new_args[j])
            {
                while (--j >= 0)
                    free(new_args[j]);
                free(new_args);
                return NULL;
            }
            j++;
            i++;
        }
    }
    new_args[j] = NULL;
    return new_args;
}

int count_args_without_redirections(char **args)
{
    int i = 0;
    int count = 0;

    while (args[i])
    {
        if ((ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], ">") == 0 ||
             ft_strcmp(args[i], ">>") == 0 || ft_strcmp(args[i], "<<") == 0)
            && args[i + 1])
        {
            i += 2;
        }
        else
        {
            count++;
            i++;
        }
    }
    return count;
}
void execute_cmd(t_cmd_exec *cmd_exec, char **envp, char **args, t_cmd *cmd)
{
    pid_t pid;
    int status;
    struct stat st;
    char **cleaned_args;

    if (!cmd_exec || !args || !cmd_exec->path)
    {
        ft_putstr_fd("Error: Invalid command\n", 2);
        cmd->last_exit_code = 127;
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        cmd->last_exit_code = 1;
        return;
    }
    
    if (pid == 0) // Processus enfant
    {
        // Appliquer les redirections
        if (handle_redirections(cmd) < 0)
        {
            exit(2);
        }
        
        // Nettoyer les arguments
        cleaned_args = remove_redirections(cmd->args);
        if (!cleaned_args)
            exit(1);
            
        execve(cmd_exec->path, cleaned_args, envp);
        
        // Si execve échoue, vérifier le type d'erreur
        if (stat(cmd_exec->path, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                ft_putstr_fd(args[0], 2);
                ft_putstr_fd(": Is a directory\n", 2);
                exit(126);
            }
            else if (!(st.st_mode & S_IXUSR))
            {
                ft_putstr_fd(args[0], 2);
                ft_putstr_fd(": Permission denied\n", 2);
                exit(126);
            }
        }
        
        ft_putstr_fd(args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    else // Processus parent
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            cmd->last_exit_code = WEXITSTATUS(status);
        else
            cmd->last_exit_code = 1;
    }
}

void exec_pipeline(t_cmd *cmds, char **envp)
{
    int     pipefd[2];
    int     prev_fd = -1;
    pid_t   pid;
    t_cmd   *current = cmds;
    int     status;
    pid_t   *pids = NULL;
    int     cmd_count = 0;
    
    // Compter les commandes pour allouer le tableau de PIDs
    t_cmd *tmp = current;
    while (tmp)
    {
        cmd_count++;
        tmp = tmp->next;
    }
    
    pids = malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
    {
        perror("malloc");
        return;
    }
    
    int cmd_index = 0;
    
    while (current)
    {
        // Créer le pipe seulement s'il y a une commande suivante
        if (current->next)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                free(pids);
                return;
            }
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(pids);
            return;
        }

        if (pid == 0) // Processus enfant
        {
            // Configurer les pipes AVANT les redirections
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (current->next)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            // Appliquer les redirections APRÈS la configuration des pipes
            if (handle_redirections(current) < 0)
                exit(2);

            // Nettoyer les arguments (enlever les redirections)
            char **cleaned_args = remove_redirections(current->args);
            if (!cleaned_args)
                exit(1);

            if (is_builtin(current))
            {
                // Pour les builtins, utiliser les args nettoyés
                t_cmd temp_cmd = *current;
                temp_cmd.args = cleaned_args;
                exec_builtin(&temp_cmd);
                
                // Libérer les args nettoyés
                int i = 0;
                while (cleaned_args[i])
                    free(cleaned_args[i++]);
                free(cleaned_args);
                exit(cmds->last_exit_code);
            }

            char *path = find_path(cmds, cleaned_args[0], envp);
            if (!path)
            {
                ft_putstr_fd(cleaned_args[0], 2);
                ft_putstr_fd(": command not found \n", 2);
                
                // Libérer les args nettoyés
                int i = 0;
                while (cleaned_args[i])
                    free(cleaned_args[i++]);
                free(cleaned_args);
                exit(127);
            }
            
            execve(path, cleaned_args, envp);
            perror("execve");
            
            // Libérer en cas d'échec de execve
            free(path);
            int i = 0;
            while (cleaned_args[i])
                free(cleaned_args[i++]);
            free(cleaned_args);
            exit(126);
        }
        else // Processus parent
        {
            pids[cmd_index++] = pid;
            
            if (prev_fd != -1)
                close(prev_fd);
                
            if (current->next)
            {
                close(pipefd[1]);
                prev_fd = pipefd[0];
            }
        }
        current = current->next;
    }

    // Attendre tous les processus enfants
    for (int i = 0; i < cmd_count; i++)
    {
        waitpid(pids[i], &status, 0);
        if (i == cmd_count - 1) // Dernière commande
        {
            if (WIFEXITED(status))
                cmds->last_exit_code = WEXITSTATUS(status);
            else
                cmds->last_exit_code = 1;
        }
    }
    
    free(pids);
}


