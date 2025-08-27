/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 19:06:02 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/27 17:12:29 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	is_builtin(char *str, int *builtin)
{
	char	*cmd_name;

	cmd_name = strip_path(str);
	if (ft_strcmp(cmd_name, "echo") == 0
		|| ft_strcmp(cmd_name, "pwd") == 0
		|| ft_strcmp(cmd_name, "env") == 0
		|| ft_strcmp(cmd_name, "exit") == 0
		|| ft_strcmp(cmd_name, "cd") == 0
		|| ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0)
	{
		*builtin = 3;
	}
}

static void	add_command(t_command **command_p, char *str, int *builtin)
{
	is_builtin(str, builtin);
	(*command_p) = malloc(sizeof(t_command));
	(*command_p)->command = ft_strdup(str);
	(*command_p)->next = NULL;
	(*command_p)->contents_p = NULL;
	(*command_p)->parameters_p = NULL;
	(*command_p)->token = NULL;
	(*command_p)->redirections = NULL;
	(*command_p)->infile = -1;
	(*command_p)->outfile = -1;
	(*command_p)->flag = 1;
	(*command_p)->token_flag = 0;
	(*command_p)->builtin = *builtin;
}

void	append_command(t_shell *shell, char *str, int builtin, t_command **temp)
{
	t_command	*last;

	if (!shell->command_p)
	{
		add_command(&shell->command_p, str, &builtin);
		*temp = shell->command_p;
	}
	else if (*temp)
	{
		add_command(&(*temp)->next, str, &builtin);
		*temp = (*temp)->next;
	}
	else
	{
		last = shell->command_p;
		while (last && last->next)
			last = last->next;
		if (last)
		{
			add_command(&last->next, str, &builtin);
			*temp = last->next;
		}
	}
}

void	append_parameter(t_command **temp, char *str)
{
	char	*dup;
	t_list	*new_node;

	dup = ft_strdup(str);
	new_node = ft_lstnew(dup);
	if (!(*temp)->contents_p)
		(*temp)->contents_p = new_node;
	else
		ft_lstadd_back(&(*temp)->contents_p, new_node);
}

void	append_token(char *token, t_command **temp)
{
	(*temp)->token = ft_strdup(token);
	(*temp)->token_flag = 1;
}
