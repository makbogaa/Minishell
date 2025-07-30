/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:30:18 by makboga           #+#    #+#             */
/*   Updated: 2025/07/30 16:00:05 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	has_slash(const char *s)
{
	return (ft_strchr(s, '/') != NULL);
}

int	run(t_command *cmd, char **args, t_shell *sh)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid == 0)
	{
		// Redirectionları setup et
		if (setup_redirections(cmd) == -1)
			exit(1);
			
		if (has_slash(cmd->command))
		{
			execve(cmd->command, args, sh->envp);
			if (errno == ENOENT)
			{
				write(2, "minishell: no such file or directory: ", 38);
				write(2, cmd->command, ft_strlen(cmd->command));
				write(2, "\n", 1);
				exit(127);
			}
			perror("execve");
			exit(126);
		}
		path = get_path(cmd->command, sh->envp);
		if (!path)
		{
			write(2, "minishell: command not found: ", 31);
			write(2, cmd->command, ft_strlen(cmd->command));
			write(2, "\n", 1);
			exit(127);
		}
		execve(path, args, sh->envp);
		perror("execve");
		free(path);
		exit(126);
	}
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
