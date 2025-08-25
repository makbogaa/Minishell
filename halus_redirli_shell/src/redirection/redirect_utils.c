/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 15:38:24 by makboga           #+#    #+#             */
/*   Updated: 2025/08/25 16:33:21 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	process_heredoc_line(char *line, int pipe_fd, t_req *req)
{
	char	*expanded_line;

	expanded_line = expand_line(line, req, 0, ft_strdup(""));
	write(pipe_fd, expanded_line, ft_strlen(expanded_line));
	write(pipe_fd, "\n", 1);
	free(line);
	free(expanded_line);
}

void	rm_rdr_tkns(t_list **content, t_list *prev, t_command *cmd)
{
	if (prev)
	{
		prev->next = (*content)->next->next;
		free((*content)->content);
		free((*content)->next->content);
		free((*content)->next);
		free(*content);
		*content = prev->next;
	}
	else
	{
		cmd->contents_p = (*content)->next->next;
		free((*content)->content);
		free((*content)->next->content);
		free((*content)->next);
		free(*content);
		*content = cmd->contents_p;
	}
}

t_redirect	*create_rdr_node(t_redirect_type type, char *filename)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->filename = ft_strdup(filename);
	redir->fd = -1;
	redir->next = NULL;
	return (redir);
}

int	handle_output_rdr(t_redirect *redir, int *last_out, t_req *req)
{
	int	fd;

	fd = open_redirect_file(redir->filename, redir->type, req, -1);
	if (fd < 0)
		return (1);
	if (*last_out != -1 && *last_out != STDOUT_FILENO)
		close(*last_out);
	*last_out = fd;
	return (0);
}

void	cleanup_file_desc(int *last_in, int *last_out)
{
	if (*last_in != -1 && *last_in != STDIN_FILENO)
		close(*last_in);
	if (*last_out != -1 && *last_out != STDOUT_FILENO)
		close(*last_out);
}
