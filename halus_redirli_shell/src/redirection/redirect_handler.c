/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:41:21 by makboga           #+#    #+#             */
/*   Updated: 2025/08/25 15:51:05 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	open_redirect_file(char *filename, t_redirect_type type,
	t_req *req, int fd)
{
	if (!filename)
	{
		ft_putstr_fd("minishell: redirect: filename is NULL\n", 2);
		if (req)
			req->exit_stat = 1;
		return (-1);
	}
	if (type == REDIRECT_IN)
		fd = open(filename, O_RDONLY);
	else if (type == REDIRECT_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == REDIRECT_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		if (req)
			req->exit_stat = 1;
	}
	return (fd);
}

static int	handle_input_redirect(t_redirect *redir, int *last_in, t_req *req)
{
	int	fd;

	fd = open_redirect_file(redir->filename, REDIRECT_IN, req, -1);
	if (fd < 0)
		return (1);
	if (*last_in != -1 && *last_in != STDIN_FILENO)
		close(*last_in);
	*last_in = fd;
	return (0);
}

static int	handle_heredoc_redirect(t_redirect *redir, int *last_in, t_req *req)
{
	int	heredoc_fd;

	heredoc_fd = handle_heredoc(redir->filename, req);
	if (heredoc_fd == -1)
		return (1);
	if (*last_in != -1 && *last_in != STDIN_FILENO)
		close(*last_in);
	*last_in = heredoc_fd;
	return (0);
}

static int	process_single_redirect(t_redirect *redir,
	int *last_in, int *last_out, t_req *req)
{
	if (redir->type == REDIRECT_IN
		&& handle_input_redirect(redir, last_in, req))
	{
		cleanup_file_desc(last_in, last_out);
		return (1);
	}
	if ((redir->type == REDIRECT_OUT || redir->type == REDIRECT_APPEND)
		&& handle_output_rdr(redir, last_out, req))
	{
		cleanup_file_desc(last_in, last_out);
		return (1);
	}
	if (redir->type == REDIRECT_HEREDOC
		&& handle_heredoc_redirect(redir, last_in, req))
	{
		cleanup_file_desc(last_in, last_out);
		return (1);
	}
	return (0);
}

int	apply_redirects(t_pipeline_data *data)
{
	t_redirect	*redir;
	int			last_in;
	int			last_out;

	redir = data->current_cmd->redirections;
	last_in = -1;
	last_out = -1;
	while (redir)
	{
		if (process_single_redirect(redir, &last_in, &last_out, data->req))
			return (1);
		redir = redir->next;
	}
	if (last_in != -1)
		data->current_cmd->infile = last_in;
	if (last_out != -1)
		data->current_cmd->outfile = last_out;
	return (0);
}
