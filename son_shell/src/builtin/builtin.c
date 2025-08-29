/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:01:49 by makboga           #+#    #+#             */
/*   Updated: 2025/08/29 20:13:23 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	has_slash(const char *s)
{
	return (ft_strchr(s, '/') != NULL);
}

static void	check_command_validity(t_command *cmd, struct stat *buffer)
{
	if (stat(cmd->command, buffer) == -1)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd->command, ft_strlen(cmd->command));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		exit(127);
	}
	if (access(cmd->command, X_OK) == -1)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, "Permission denied\n", 18);
		exit(126);
	}
	if (S_ISDIR(buffer->st_mode))
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, "Is a directory\n", 15);
		exit(126);
	}
	if (!(buffer->st_mode & S_IXUSR))
	{
		write(STDERR_FILENO, "Permission denied\n", 19);
		exit(126);
	}
}

static int	handle_slash_command(t_command *cmd, char **args, t_shell *sh)
{
	struct stat	buffer;

	check_command_validity(cmd, &buffer);
	execve(cmd->command, args, sh->envp);
	perror("execve");
	exit(127);
}

static int	handle_regular_command(t_command *cmd, char **args, t_shell *sh)
{
	char	*path;

	path = get_path(cmd->command, sh->envp);
	if (!path)
	{
		write(STDERR_FILENO, cmd->command, ft_strlen(cmd->command));
		write(STDERR_FILENO, ": command not found\n", 20);
		free_shell(sh);
		free(args);
		exit(127);
	}
	if (!cmd->command[0])
		exit(0);
	execve(path, args, sh->envp);
	perror("execve");
	free(path);
	exit(127);
}

int	run(t_command *cmd, char **args, t_shell *sh)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		child_signal();
		if (has_slash(cmd->command))
			handle_slash_command(cmd, args, sh);
		else
			handle_regular_command(cmd, args, sh);
	}
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	ignore_signals();
	waitpid(pid, &status, 0);
	parent_signal();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
