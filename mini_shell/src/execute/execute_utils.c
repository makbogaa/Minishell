/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:10:22 by makboga           #+#    #+#             */
/*   Updated: 2025/07/22 13:33:58 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*build_command_string(t_command *cmd)
{
	char			*result;
	t_parameters	*param;

	result = ft_strdup(cmd->command);
	param = cmd->parameters_p;
	while (param)
	{
		result = ft_strjoin_free(result, " ");
		result = ft_strjoin_free(result, param->parameter);
		param = param->next;
	}
	return (result);
}

void execute_single_command(t_shell *shell, char **argv)
{
	char *path;

	path = get_path(argv[0], shell->envp);
	if (!path)
	{
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return;
	}
	if (fork() == 0)
	{
		execve(path, argv, shell->envp);
		perror("execve failed");
		exit(1);
	}
	wait(NULL);
	free(path);
}

void run_commands(t_shell *shell)
{
	char	*full_command;
	char	**pipe_commands;
	int		i;
	int		pipe_count;
	char **argv;

	if (!shell->command_p || !shell->command_p->command)
		return;
	full_command = build_command_string(shell->command_p);
	if (!full_command)
		return;
	pipe_count = 0;
	i = 0;
	while (full_command[i])
	{
		if (full_command[i] == '|')
			pipe_count++;
		i++;
	}
	if (pipe_count == 0)
	{
		argv = ft_split(full_command, ' ');
		if (!argv || !argv[0])
		{
			free(full_command);
			return;
		}
		if (builtin_exe(shell, argv) == -1)
			execute_single_command(shell, argv);
		free_argv(argv);
	}
	else
	{
		pipe_commands = ft_split(full_command, '|');
		if (!pipe_commands)
		{
			free(full_command);
			return;
		}
		i = 0;
		while (pipe_commands[i])
			i++;
		execute_commands(shell, pipe_commands, i);
		ft_free_split(pipe_commands);
	}
	free(full_command);
}

