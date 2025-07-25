/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:18:27 by makboga           #+#    #+#             */
/*   Updated: 2025/07/23 18:44:45 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **get_params(t_command *command)
{
    int i;
    char **params;
    t_parameters *param;
    t_parameters *tmp;

    param = command->parameters_p;
    tmp = param;
    i = 0;
    while(tmp && tmp->parameter)
    {
        i++;
        tmp = tmp->next;
    }
    params = malloc(sizeof(char *) * (i + 2));
    if (!params)
    {
        perror("malloc");
        return NULL;
    }
    params[0] = command->command;
    i = 1;
    tmp = param;
    while(tmp && tmp->parameter)
    {
        params[i] = tmp->parameter;
        i++;
        tmp = tmp->next;
    }
    params[i] = NULL;
    return params;
}
char *strip_path(char *cmd)
{
    char *slash = ft_strrchr(cmd, '/');
    if (slash)
        return slash + 1;
    return cmd;
}

void execute(t_shell *shell)
{
    char **params;
    char **commands_pipes;
    int i_pipes;
    char *cmd_name;

    if (ft_strchr(shell->prompt, '|'))
    {
        commands_pipes = ft_split(shell->prompt, '|');
        i_pipes = 0;
        while (commands_pipes[i_pipes] && commands_pipes)
            i_pipes++;
        execute_commands(shell, commands_pipes, i_pipes);
        ft_free_split(commands_pipes);
        return;
    }
    params = get_params(shell->command_p);
	printf("Executing command: %s\n", shell->command_p->command);
    if (shell->command_p->builtin == 2 || shell->command_p->builtin == 1)
		run(shell->command_p, params,shell);
	else if(shell->command_p->builtin == 3)
	{
        cmd_name = strip_path(shell->command_p->command);
		if (ft_strcmp(cmd_name, "echo") == 0)
            builtin_echo(params);
		else if(ft_strcmp(cmd_name,"pwd") == 0)
			builtin_pwd();
		else if(ft_strcmp(cmd_name,"exit") == 0)
			builtin_exit(params);
		else if(ft_strcmp(cmd_name,"env") == 0)
			builtin_env(shell->envp);
        else if(ft_strcmp(cmd_name,"cd") == 0)
            builtin_cd(shell, params);
        else if (ft_strcmp(cmd_name, "export") == 0)
            builtin_export(&shell->envp, params);
        else if (ft_strcmp(cmd_name, "unset") == 0)
            builtin_unset(shell, params[1]);
        else
            printf("command not found: %s\n", cmd_name);
		free(params);
	}
}

