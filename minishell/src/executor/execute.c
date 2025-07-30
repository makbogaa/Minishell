/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:59:51 by makboga           #+#    #+#             */
/*   Updated: 2025/07/30 17:59:34 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

void handle_command_execution(t_shell *shell, char **params)
{
    char *cmd_name;
    
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
    }
}

int setup_and_restore_redirections(t_shell *shell, int *original_stdin, int *original_stdout, int setup_mode)
{
    if (!shell->command_p || !shell->command_p->redirections)
        return 0;
        
    if (setup_mode) // Setup mode
    {
        *original_stdin = dup(STDIN_FILENO);
        *original_stdout = dup(STDOUT_FILENO);
        
        if (setup_redirections(shell->command_p) == -1)
        {
            if (*original_stdin != -1)
            {
                dup2(*original_stdin, STDIN_FILENO);
                close(*original_stdin);
            }
            if (*original_stdout != -1)
            {
                dup2(*original_stdout, STDOUT_FILENO);
                close(*original_stdout);
            }
            return -1;
        }
    }
    else // Restore mode
    {
        if (*original_stdin != -1)
        {
            dup2(*original_stdin, STDIN_FILENO);
            close(*original_stdin);
        }
        if (*original_stdout != -1)
        {
            dup2(*original_stdout, STDOUT_FILENO);
            close(*original_stdout);
        }
    }
    return 0;
}

int	check_pipe_syntax(char *str)
{
	int	i;
	int	count;
	int	in_quotes;
	char quote_char;

	i = 0;
	count = 0;
	in_quotes = 0;
	quote_char = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_char = str[i];
		}
		else if (in_quotes && str[i] == quote_char)
		{
			in_quotes = 0;
			quote_char = 0;
		}
		else if (!in_quotes && str[i] == '|')
		{
			count++;
		}
		else if (!in_quotes && str[i] != ' ' && str[i] != '\t')
			count = 0;
		if (count > 1)
		{
			write(2, "zsh: parse error near `||'\n", 28);
			return (2);
		}
		i++;
	}
	if (!in_quotes && (str[0] == '|' || str[i - 1] == '|'))
	{
		write(2, "zsh: parse error near `|'\n", 27);
		return (2);
	}
	return (0);
}

int has_pipe_outside_quotes(char *str)
{
    int i = 0;
    int in_quotes = 0;
    char quote_char = 0;
    
    while (str[i])
    {
        if (!in_quotes && (str[i] == '"' || str[i] == '\''))
        {
            in_quotes = 1;
            quote_char = str[i];
        }
        else if (in_quotes && str[i] == quote_char)
        {
            in_quotes = 0;
            quote_char = 0;
        }
        else if (!in_quotes && str[i] == '|')
        {
            return 1; // Pipe found outside quotes
        }
        i++;
    }
    return 0; // No pipe outside quotes
}

void execute(t_shell *shell)
{
    char **params;
    char **commands_pipes;
    int i_pipes;
    int original_stdin = -1;
    int original_stdout = -1;

    if (has_pipe_outside_quotes(shell->prompt))
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
    if (setup_and_restore_redirections(shell, &original_stdin, &original_stdout, 1) == -1)
        return;
    params = get_params(shell->command_p);
    handle_command_execution(shell, params);
    free(params);
    // File descriptor'ları restore et
    setup_and_restore_redirections(shell, &original_stdin, &original_stdout, 0);
}

void execute_single_command(t_shell *shell)
{
    char **params;
    int original_stdin = -1;
    int original_stdout = -1;

    if (!shell || !shell->command_p)
        return;
    
    // Pipe içinde de redirection setup et
    if (setup_and_restore_redirections(shell, &original_stdin, &original_stdout, 1) == -1)
        return;
    
    params = get_params(shell->command_p);
    handle_command_execution(shell, params);
    free(params);
    
    // Redirection restore
    setup_and_restore_redirections(shell, &original_stdin, &original_stdout, 0);
}

