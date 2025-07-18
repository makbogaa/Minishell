/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:27 by makboga           #+#    #+#             */
/*   Updated: 2025/07/17 19:13:32 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
	int	i;

	shell->prompt = NULL;
	shell->current_dir = NULL;
	shell->display_info = NULL;
	shell->envp = NULL;

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

void get_display_info(t_shell *shell)
{
	if (shell->display_info)
	{
		free(shell->display_info);
		shell->display_info = NULL;
	}
	shell->display_info = ft_strdup("makboga&&mdalkilic$ ");
	if (!shell->display_info)
	{
		perror("ft_strdup");
		exit(EXIT_FAILURE);
	}
}

void start_minishell(t_shell *shell)
{
    char *input;
    char **argv;
    char **commands;
    int n;
    int status;

    while (1)
    {
        free(shell->current_dir);
        shell->current_dir = getcwd(NULL, 0);
        if (!shell->current_dir)
        {
            perror("getcwd");
            shell->current_dir = ft_strdup("minishell");
            if (!shell->current_dir)
            {
                perror("ft_strdup");
                exit(EXIT_FAILURE);
            }
        }
        get_display_info(shell);
        input = readline(shell->display_info);
        if (!input)
        {
            printf("\nexit\n");
            break;
        }
        if (ft_strlen(input) > 0)
            add_history(input);
        if (ft_strchr(input, '|'))
        {
            commands = ft_split(input, '|');
            if (!commands)
            {
                free(input);
                continue;
            }
            n = 0;
            while (commands[n])
                n++;
            execute_commands(shell, commands, n);
            ft_free_split(commands);
        }
        else
        {
            commands = malloc(sizeof(char *) * 2);
            if (!commands)
            {
                free(input);
                continue;
            }
            commands[0] = ft_strdup(input);
            commands[1] = NULL;
            argv = ft_split(input, ' ');
            if (argv && argv[0])
            {
                status = builtin_exe(shell, argv);
                if (status == -1)
                    execute_commands(shell, commands, 1);
            }
            ft_free_split(argv);
            ft_free_split(commands);
        }
        free(input);
        input = NULL;
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