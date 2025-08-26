/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:27 by makboga           #+#    #+#             */
/*   Updated: 2025/08/26 15:15:41 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_envp(t_shell *shell, char **envp, int i)
{
	while (envp[i])
		i++;
	shell->envp = malloc(sizeof(char *) * (i + 1));
	if (!shell->envp)
	{
		perror("malloc");
		free_shell(shell);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (envp[i])
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
		{
			perror("strdup");
			free_shell(shell);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	shell->envp[i] = NULL;
	increment_shlvl(shell);
}

void	init_shell(t_shell *shell, char **envp)
{
	int	i;

	i = 0;
	shell->prompt = NULL;
	shell->current_dir = NULL;
	shell->display_info = NULL;
	shell->command_p = NULL;
	shell->builtin[0] = ft_strdup("exit");
	shell->builtin[1] = ft_strdup("echo");
	shell->builtin[2] = ft_strdup("env");
	shell->builtin[3] = ft_strdup("export");
	shell->builtin[4] = ft_strdup("unset");
	shell->builtin[5] = ft_strdup("pwd");
	shell->builtin[6] = ft_strdup("cd");
	shell->builtin[7] = NULL;
	shell->tokens[0] = ft_strdup("|");
	shell->tokens[1] = ft_strdup(">");
	shell->tokens[2] = ft_strdup("<");
	shell->tokens[3] = ft_strdup(">>");
	shell->tokens[4] = ft_strdup("<<");
	shell->tokens[5] = NULL;
	shell->last_exit_code = 0;
	get_hostname(shell);
	init_envp(shell, envp, i);
}

void	start_minishell(t_shell *shell)
{
	while (1)
	{
		if (shell->current_dir)
			free(shell->current_dir);
		shell->current_dir = getcwd(NULL, 0);
		if (!shell->current_dir)
			shell->current_dir = ft_strdup(".");
		get_display_info(shell);
		if (!get_prompt(shell))
		{
			if (shell->command_p)
				free_command(shell);
			break ;
		}
		if (shell->command_p)
		{
			execute(shell);
			free_command(shell);
			shell->command_p = NULL;
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		exit_code;

	(void)argv;
	if (argc != 1)
	{
		write(STDERR_FILENO, "Error: No arguments allowed\n", 28);
		return (1);
	}
	ft_memset(&shell, 0, sizeof(t_shell));
	init_shell(&shell, envp);
	start_minishell(&shell);
	exit_code = shell.last_exit_code;
	free_shell(&shell);
	return (exit_code);
}
