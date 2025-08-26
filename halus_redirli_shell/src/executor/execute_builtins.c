/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:06:39 by makboga           #+#    #+#             */
/*   Updated: 2025/08/26 15:10:58 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_command_execution(t_shell *shell, char **params)
{
	char	*cmd_name;
	int		should_exit;

	if (shell->command_p->builtin == 2 || shell->command_p->builtin == 1)
	{
		shell->last_exit_code = run(shell->command_p, params, shell);
		return (0);
	}
	else if (shell->command_p->builtin == 3)
	{
		cmd_name = ft_strrchr(shell->command_p->command, '/');
		if (cmd_name)
			cmd_name++;
		else
			cmd_name = shell->command_p->command;
		should_exit = handle_builtin_command(shell, params, cmd_name);
		return (should_exit);
	}
	return (0);
}

static void	restore_std_fds(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

static void	execute_with_redirections(t_shell *shell)
{
	char	**params;
	int		original_stdin;
	int		original_stdout;
	int		should_exit;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (setup_redirections(shell->command_p, shell) == -1)
	{
		restore_std_fds(original_stdin, original_stdout);
		return ;
	}
	params = get_params(shell->command_p);
	should_exit = handle_command_execution(shell, params);
	free(params);
	restore_std_fds(original_stdin, original_stdout);
	if (should_exit)
	{
		free_shell(shell);
		exit(shell->last_exit_code);
	}
}

void	handle_single_command_exec(t_shell *shell)
{
	char	**params;
	int		should_exit;

	if (!shell->command_p)
		return ;
	if (!shell->command_p->redirections)
	{
		params = get_params(shell->command_p);
		should_exit = handle_command_execution(shell, params);
		free(params);
		if (should_exit)
		{
			free_shell(shell);
			exit(shell->last_exit_code);
		}
		return ;
	}
	execute_with_redirections(shell);
}

void	execute_single_command(t_shell *shell)
{
	char	**params;
	int		should_exit;

	if (!shell || !shell->command_p)
		return ;
	params = get_params(shell->command_p);
	should_exit = handle_command_execution(shell, params);
	free(params);
	if (should_exit)
	{
		free_shell(shell);
		exit(shell->last_exit_code);
	}
}
