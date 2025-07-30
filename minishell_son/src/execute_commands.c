/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:18:27 by makboga           #+#    #+#             */
/*   Updated: 2025/07/30 16:40:56 by makboga          ###   ########.fr       */
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

int	check_pipe_syntax(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '|')
			count++;
		else if (str[i] != ' ' && str[i] != '\t')
			count = 0;
		if (count > 1)
		{
			write(2, "zsh: parse error near `||'\n", 28);
			return (2);
		}
		i++;
	}
	if (str[0] == '|' || str[i - 1] == '|')
	{
		write(2, "zsh: parse error near `|'\n", 27);
		return (2);
	}
	return (0);
}

void execute(t_shell *shell)
{
    char **params;
    char **commands_pipes;
    int i_pipes;
    char *cmd_name;
    int original_stdin = -1;
    int original_stdout = -1;

    if (ft_strchr(shell->prompt, '|'))
    {
        if (check_pipe_syntax(shell->prompt) != 0)
            return;
        commands_pipes = ft_split(shell->prompt, '|');
        i_pipes = 0;
        while (commands_pipes[i_pipes])
            i_pipes++;
        execute_commands(shell, commands_pipes, i_pipes);
        ft_free_split(commands_pipes);
        return;
    }
    
    // Redirection setup (sadece pipe olmayan durumlarda)
    if (shell->command_p && shell->command_p->redirections)
    {
        original_stdin = dup(STDIN_FILENO);
        original_stdout = dup(STDOUT_FILENO);
        
        if (setup_redirections(shell->command_p) == -1)
        {
            if (original_stdin != -1)
            {
                dup2(original_stdin, STDIN_FILENO);
                close(original_stdin);
            }
            if (original_stdout != -1)
            {
                dup2(original_stdout, STDOUT_FILENO);
                close(original_stdout);
            }
            return;
        }
    }
    
    params = get_params(shell->command_p);
	//printf("Executing command: %s\n", shell->command_p->command);
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
    
    // File descriptor'ları restore et
    if (shell->command_p && shell->command_p->redirections)
    {
        if (original_stdin != -1)
        {
            dup2(original_stdin, STDIN_FILENO);
            close(original_stdin);
        }
        if (original_stdout != -1)
        {
            dup2(original_stdout, STDOUT_FILENO);
            close(original_stdout);
        }
    }
}

void execute_single_command(t_shell *shell)
{
    char **params;
    char *cmd_name;

    if (!shell || !shell->command_p)
        return;
    
    params = get_params(shell->command_p);
    if (shell->command_p->builtin == 2 || shell->command_p->builtin == 1)
        run(shell->command_p, params, shell);
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

