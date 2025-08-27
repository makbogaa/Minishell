/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:06:59 by makboga           #+#    #+#             */
/*   Updated: 2025/08/27 17:13:43 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	execute_child_process(t_shell *shell, char *command,
		t_pipe_info pipe_info)
{
	t_shell	tmp_shell;
	char	*trimmed_command;

	child_signal();
	ft_memset(&tmp_shell, 0, sizeof(t_shell));
	tmp_shell.envp = shell->envp;
	trimmed_command = ft_strtrim(command, " \t\n\r");
	tmp_shell.prompt = trimmed_command;
	parse_prompt(&tmp_shell);
	setup_child_pipes(pipe_info.pipefds, pipe_info.n, pipe_info.i);
	if (tmp_shell.command_p && tmp_shell.command_p->redirections)
	{
		if (setup_redirections(tmp_shell.command_p, &tmp_shell) != 0)
		{
			free_command(&tmp_shell);
			if (tmp_shell.prompt)
				free(tmp_shell.prompt);
			exit(tmp_shell.last_exit_code);
		}
	}
	execute_single_command(&tmp_shell);
	free_shell(&tmp_shell);
	free(pipe_info.pipefds);
	free(command);
	// free_command(&tmp_shell);
	// if (tmp_shell.prompt)
	// 	free(tmp_shell.prompt);
	exit(tmp_shell.last_exit_code);
}

static void	fork_and_exec(t_shell *shell, char **commands, int n, int *pipefds)
{
	pid_t		pids[1024];
	int			i;
	t_pipe_info	pipe_info;

	i = 0;
	ignore_signals();
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			pipe_info.pipefds = pipefds;
			pipe_info.n = n;
			pipe_info.i = i;
			execute_child_process(shell, commands[i], pipe_info);
		}
		else if (pids[i] < 0)
			break ;
		i++;
	}
	cleanup_and_wait(pipefds, n, pids[n - 1], shell);
	parent_signal();
}

static void	execute_commands(t_shell *shell, char **commands, int n)
{
	int	*pipefds;

	if (n < 2)
		return ;
	if (create_pipes(&pipefds, n) == -1)
		return ;
	fork_and_exec(shell, commands, n, pipefds);
}

static void	handle_pipe_commands(t_shell *shell)
{
	char	**commands_pipes;
	int		i_pipes;

	if (check_pipe_syntax(shell->prompt) != 0)
		return ;
	commands_pipes = ft_split(shell->prompt, '|');
	i_pipes = 0;
	while (commands_pipes[i_pipes])
		i_pipes++;
	if (i_pipes > 1)
		execute_commands(shell, commands_pipes, i_pipes);
	ft_free_split(commands_pipes);
}

void	execute(t_shell *shell)
{
	if (has_pipe_outside_quotes(shell->prompt))
		handle_pipe_commands(shell);
	else
		handle_single_command_exec(shell);
}
