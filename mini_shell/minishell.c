/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:27 by makboga           #+#    #+#             */
/*   Updated: 2025/07/21 20:40:19 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
    
    shell->prompt = NULL;
	shell->current_dir = NULL;
	shell->display_info = NULL;
    shell->command_p = NULL;
    shell->builtin = malloc(sizeof(char *) * 2);
	if (!shell->builtin)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	shell->builtin[0] = ft_strdup("echo");
	if (!shell->builtin[0])
		exit(EXIT_FAILURE);
	shell->builtin[1] = NULL;
    get_hostname(shell);
	///
    int	i;
	i = 0;
	while (envp[i])
		i++;
	shell->envp = malloc(sizeof(char *) * (i + 1));
	if (!shell->envp)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (envp[i])
	{
		shell->envp[i] = strdup(envp[i]);
		if (!shell->envp[i])
		{
			perror("strdup");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	shell->envp[i] = NULL;
}

void start_minishell(t_shell *shell)
{

    while (1)
    {
        shell->current_dir = getcwd(NULL, 0);
        get_display_info(shell);
        get_prompt(shell);
		run_commands(shell);
    }
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;

	init_shell(&shell, envp);
	start_minishell(&shell);
	free_shell(&shell);
	return (0);
}