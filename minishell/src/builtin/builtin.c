/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:30:18 by makboga           #+#    #+#             */
/*   Updated: 2025/08/12 18:59:01 by makboga          ###   ########.fr       */
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
	struct stat buffer;

	pid = fork();
	if (pid == 0)
	{
		// Redirectionları setup et
		if (setup_redirections(cmd) == -1)
			exit(1);
			
		if (has_slash(cmd->command))
		{
			stat(cmd->command, &buffer);
			if (errno == ENOENT)
            {
				fprintf(stderr, "minishell: %s: No such file or directory\n", cmd->command);
            	exit(127);
			}
			if (S_ISDIR(buffer.st_mode))
			{
				write(STDERR_FILENO, "minishell: ", 11);
				write(STDERR_FILENO, cmd->command, ft_strlen(cmd->command));
				write(STDERR_FILENO, " Is a directory\n", 16);
				exit(126);
			}

			if (!(buffer.st_mode & S_IXUSR)) // Çalıştırma izni yoksa
			{
				write(STDERR_FILENO, "Permission denied\n", 19);
				exit(126);
			}

			execve(cmd->command, args, sh->envp);
			perror("execve");
			exit(127);
		}
		path = get_path(cmd->command, sh->envp);
		if (!path)
		{
			write(STDERR_FILENO, "minishell: command not found: ", 31);
			write(STDERR_FILENO, cmd->command, ft_strlen(cmd->command));
			write(STDERR_FILENO, "\n", 1);
			exit(127);
		}
		if(!cmd->command[0])
			exit(0);
		execve(path, args, sh->envp);
		perror("execve");
		free(path);
		exit(127);
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
