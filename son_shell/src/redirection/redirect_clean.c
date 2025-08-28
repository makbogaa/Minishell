/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_clean.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:37:57 by makboga           #+#    #+#             */
/*   Updated: 2025/08/28 13:28:57 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_redirect	*create_redir_node_from_token(t_list **content, char *str)
{
	if (!content || !*content || !(*content)->next)
		return (NULL);
	if (ft_strcmp(str, "<") == 0)
		return (create_rdr_node(REDIRECT_IN, (*content)->next->content));
	if (ft_strcmp(str, ">") == 0)
		return (create_rdr_node(REDIRECT_OUT, (*content)->next->content));
	if (ft_strcmp(str, ">>") == 0)
		return (create_rdr_node(REDIRECT_APPEND, (*content)->next->content));
	if (ft_strcmp(str, "<<") == 0)
		return (create_rdr_node(REDIRECT_HEREDOC, (*content)->next->content));
	return (NULL);
}

static int	handle_redirect_operator(t_command *cmd, t_list **content,
	t_list *prev, char *str)
{
	t_redirect	*redir;
	t_redirect	*tail;

	redir = create_redir_node_from_token(content, str);
	if (redir)
	{
		if (!cmd->redirections)
			cmd->redirections = redir;
		else
		{
			tail = cmd->redirections;
			while (tail->next)
				tail = tail->next;
			tail->next = redir;
		}
		rm_rdr_tkns(content, prev, cmd);
		return (1);
	}
	return (0);
}

static void	process_cmd_redirections(t_command *cmd)
{
	t_list		*content;
	t_list		*prev;
	char		*str;

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

void	setup_heredoc_tty(int *saved_stdin, int *saved_stdout)
{
	int	tty_fd;

	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	tty_fd = open("/dev/tty", O_RDWR);
	if (tty_fd != -1)
	{
		dup2(tty_fd, STDIN_FILENO);
		dup2(tty_fd, STDOUT_FILENO);
		close(tty_fd);
	}
}
