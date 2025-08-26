/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:23:54 by makboga           #+#    #+#             */
/*   Updated: 2025/08/26 19:00:57 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_builtin_tokens(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < 8 && shell->builtin[i])
	{
		free(shell->builtin[i]);
		shell->builtin[i] = NULL;
		i++;
	}
	i = 0;
	while (i < 8 && shell->tokens[i])
	{
		free(shell->tokens[i]);
		shell->tokens[i] = NULL;
		i++;
	}
}

void	free_shell_arrays(t_shell *shell)
{
	int	i;

	if (!shell)
		return ;
	free_builtin_tokens(shell);
	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
		{
			free(shell->envp[i]);
			shell->envp[i] = NULL;
			i++;
		}
		free(shell->envp);
		shell->envp = NULL;
	}
}

static void	free_command_params(t_command *command)
{
	t_list	*param;
	t_list	*next;

	param = command->contents_p;
	while (param)
	{
		next = param->next;
		if (param->content)
			free(param->content);
		free(param);
		param = next;
	}
	command->contents_p = NULL;
}

void	free_single_command(t_command *command)
{
	if (!command)
		return ;
	if (command->contents_p)
		free_command_params(command);
	if (command->redirections)
	{
		free_redirections(command->redirections);
		command->redirections = NULL;
	}
	if (command->command)
	{
		free(command->command);
		command->command = NULL;
	}
	if (command->token)
	{
		free(command->token);
		command->token = NULL;
	}
}

void	free_command(t_shell *shell)
{
	t_command	*current;
	t_command	*next;

	if (shell->command_p)
	{
		current = shell->command_p;
		while (current)
		{
			next = current->next;
			free_single_command(current);
			free(current);
			current = next;
		}
		shell->command_p = NULL;
	}
}
