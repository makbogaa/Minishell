/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haloztur <haloztur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:45:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/22 22:29:25 by haloztur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	cleanup_params(t_list *param, t_list *to_remove)
{
	free(param->content);
	free(param);
	free(to_remove->content);
	free(to_remove);
}

static void	update_param_links(t_command *cmd, t_list *prev_param,
		t_list *next_param)
{
	if (prev_param)
		prev_param->next = next_param;
	else
		cmd->contents_p = next_param;
}

int	handle_param_redirect(t_command *cmd, t_list *param,
		t_list *prev_param)
{
	t_redirect		*redirect;
	t_list	*next_param;
	t_list	*to_remove;

	if (!param->content || !is_redirect_token(param->content)
		|| !param->next || !param->next->content)
		return (0);
	redirect = create_redirect(get_redirect_type(param->content),
			param->next->content);
	if (!redirect)
		return (0);
	add_redirect(cmd, redirect);
	to_remove = param->next;
	next_param = to_remove->next;
	update_param_links(cmd, prev_param, next_param);
	cleanup_params(param, to_remove);
	return (1);
}

void	handle_token_redirect(t_command *cmd, t_command *next_cmd)
{
	if (next_cmd->token_flag && next_cmd->token
		&& is_redirect_token(next_cmd->token))
	{
		cmd->token = ft_strdup(next_cmd->token);
		cmd->token_flag = 1;
		if (next_cmd->token)
			free(next_cmd->token);
	}
	cmd->next = next_cmd->next;
	if (next_cmd->command)
		free(next_cmd->command);
	free(next_cmd);
}

void	process_redirections(t_shell *shell)
{
	t_command	*cmd;

	cmd = shell->command_p;
	while (cmd)
	{
		if (cmd->contents_p)
			process_param_redirections(cmd);
		process_token_redirections(cmd);
		cmd = cmd->next;
	}
}
