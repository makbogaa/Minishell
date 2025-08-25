/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_clean.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:37:57 by makboga           #+#    #+#             */
/*   Updated: 2025/08/25 16:15:22 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	apply_file_descriptors(t_command *cmd)
{
	if (cmd->infile != -1)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (cmd->outfile != -1)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
}

int	setup_redirections(t_command *cmd, t_shell *shell)
{
	t_req			req;
	t_pipeline_data	data;

	if (!cmd || !cmd->redirections)
		return (0);
	cmd->infile = -1;
	cmd->outfile = -1;
	req.exit_stat = 0;
	req.heredoc_interrupted = 0;
	req.shell = shell;
	data.current_cmd = cmd;
	data.req = &req;
	if (apply_redirects(&data) != 0)
	{
		shell->last_exit_code = req.exit_stat;
		return (1);
	}
	apply_file_descriptors(cmd);
	return (0);
}

static int	handle_redirect_operator(t_command *cmd, t_list **content,
	t_list *prev, char *str)
{
	t_redirect	*redir;

	redir = NULL;
	if (ft_strcmp(str, "<") == 0 && (*content)->next)
		redir = create_rdr_node(REDIRECT_IN, (*content)->next->content);
	else if (ft_strcmp(str, ">") == 0 && (*content)->next)
		redir = create_rdr_node(REDIRECT_OUT, (*content)->next->content);
	else if (ft_strcmp(str, ">>") == 0 && (*content)->next)
		redir = create_rdr_node(REDIRECT_APPEND, (*content)->next->content);
	else if (ft_strcmp(str, "<<") == 0 && (*content)->next)
		redir = create_rdr_node(REDIRECT_HEREDOC, (*content)->next->content);
	if (redir)
	{
		redir->next = cmd->redirections;
		cmd->redirections = redir;
		rm_rdr_tkns(content, prev, cmd);
		return (1);
	}
	return (0);
}

static void	process_cmd_redirections(t_command *cmd)
{
	t_list	*content;
	t_list	*prev;
	char	*str;

	content = cmd->contents_p;
	prev = NULL;
	while (content && content->next)
	{
		str = (char *)content->content;
		if (handle_redirect_operator(cmd, &content, prev, str))
			continue ;
		prev = content;
		content = content->next;
	}
}

void	process_redirections(t_shell *shell)
{
	t_command	*cmd;

	if (!shell || !shell->command_p)
		return ;
	cmd = shell->command_p;
	while (cmd)
	{
		process_cmd_redirections(cmd);
		cmd = cmd->next;
	}
}
