/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haloztur <haloztur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:30:01 by haloztur          #+#    #+#             */
/*   Updated: 2025/08/20 15:39:08 by haloztur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	close(STDIN_FILENO);
}

char	*expand_line(char *line, t_req *req, int i, char *expanded)
{
	char	*tmp;
	char	*new_exp;
	char	c[2];

	while (line[i])
	{
		if (line[i] == '$')
		{
			tmp = expand_token_var(line, &i, req);
			new_exp = ft_strjoin(expanded, tmp);
			free(expanded);
			free(tmp);
			expanded = new_exp;
		}
		else
		{
			c[0] = line[i++];
			c[1] = '\0';
			new_exp = ft_strjoin(expanded, c);
			free(expanded);
			expanded = new_exp;
		}
	}
	return (expanded);
}

void	handle_heredoc_line(char *line, const char *delimiter,
	int pipe_fd[2], t_req *req)
{
	char	*expanded;

	if (!line)
	{
		free_cmd(req->cur_cmd);
		free_cmd(req->cmds);
		free_req(req);
		close(pipe_fd[1]);
		exit(130);
	}
	expanded = expand_line(line, req, 0, ft_strdup(""));
	if (!ft_strncmp(expanded, delimiter, ft_strlen(delimiter) + 1))
	{
		free(expanded);
		free(line);
		close(pipe_fd[1]);
		free_cmd(req->cur_cmd);
		free_cmd(req->cmds);
		free_req(req);
		exit(0);
	}
	write(pipe_fd[1], expanded, ft_strlen(expanded));
	write(pipe_fd[1], "\n", 1);
	free(expanded);
	free(line);
}