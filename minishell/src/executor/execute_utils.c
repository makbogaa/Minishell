/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:59:28 by makboga           #+#    #+#             */
/*   Updated: 2025/07/30 17:23:25 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *get_path(char *cmd, char **envp)
{
	char **paths;
	char *full_path;
	int i;

	if (!cmd || !envp)
		return NULL;
	if (cmd[0] == '/' && access(cmd, X_OK) == 0)
		return ft_strdup(cmd);
	while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
		envp++;
	if (!*envp)
		return NULL;
	paths = ft_split(*envp + 5, ':');
	if (!paths)
		return NULL;
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return full_path;
		}
		free(full_path);
		i++;
	}
	ft_free_split(paths);
	return NULL;
}

void close_pipes(int *pipefds, int count)
{
    int i = 0;
    while (i < count)
    {
        close(pipefds[i]);
        i++;
    }
}

void setup_child_fds(int *pipefds, int n, int i)
{
    if (i != 0)
        dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
    if (i != n - 1)
        dup2(pipefds[i * 2 + 1], STDOUT_FILENO);

    close_pipes(pipefds, 2 * (n - 1));
}

void execute_commands(t_shell *shell, char **commands, int n)
{
    int *pipefds = NULL;
    pid_t pid;
    int i;

    if (n > 1)
    {
        pipefds = malloc(sizeof(int) * 2 * (n - 1));
        if (!pipefds)
            return;
        i = 0;
        while (i < n - 1)
        {
            if (pipe(pipefds + i * 2) == -1)
            {
                perror("pipe");
                free(pipefds);
                return;
            }
            i++;
        }
    }
    i = 0;
    while (i < n)
    {
        pid = fork();
        if (pid == 0)
        {
            if (pipefds)
                setup_child_fds(pipefds, n, i);
            t_shell tmp_shell;
            ft_memset(&tmp_shell, 0, sizeof(t_shell));
            tmp_shell.envp = shell->envp;
            tmp_shell.prompt = ft_strdup(commands[i]);
            parse_prompt(&tmp_shell);
            execute_single_command(&tmp_shell);
            free_shell(&tmp_shell);
            exit(0);
        }
        else if (pid < 0)
        {
            perror("fork");
            break;
        }
        i++;
    }
    if (pipefds)
    {
        close_pipes(pipefds, 2 * (n - 1));
        free(pipefds);
    }
    i = 0;
    while (i < n)
    {
        wait(NULL);
        i++;
    }
}
