/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:30:18 by makboga           #+#    #+#             */
/*   Updated: 2025/07/23 20:00:59 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int run(t_command *command, char **params, t_shell *shell)
{
    pid_t pid;
    int status;
    char *cmd_path;

    pid = fork();
    if (pid == 0)
    {
        cmd_path = get_path(strip_path(command->command), shell->envp);
        if (!cmd_path)
        {
            printf("Command not found: %s\n", command->command);
            free_shell(shell);
            exit(127);
        }
        execve(cmd_path, params, shell->envp);
        perror("execve");
        free(cmd_path);
        free_shell(shell);
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
        waitpid(pid, &status, 0);
    else
    {
        perror("fork");
        return -1;
    }
    return 0;
}


// if ((*command)->builtin) == 1)
        //     return builtin_echo(argv);
        // else if (ft_strcmp(argv[0], "pwd") == 0)
        //     return builtin_pwd();
        // else if (ft_strcmp(argv[0], "env") == 0)
        //     return builtin_env(shell->envp);
        // else if (ft_strcmp(argv[0], "exit") == 0)
        // {
        //     builtin_exit(argv);
        //     return 0;
        // }
        // else if (ft_strcmp(argv[0], "export") == 0)
        //     return builtin_export(&shell->envp, argv);
        // else if (ft_strcmp(argv[0], "unset") == 0)
        //     return builtin_unset(shell, argv[1]);
        // else if (ft_strcmp(argv[0], "cd") == 0)
        //     return builtin_cd(shell, argv);
        // else
        //     return (-1);