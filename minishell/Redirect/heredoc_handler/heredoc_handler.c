/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haloztur <haloztur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 19:22:42 by haloztur          #+#    #+#             */
/*   Updated: 2025/08/17 20:15:15 by haloztur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

static void	do_heredoc_child(const char *delimiter, int pipe_fd[2], t_req *req)
{
	char	*line;

	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		handle_heredoc_line(line, delimiter, pipe_fd, req);
	}
}

static int	start_heredoc_process(const char *delimiter,
	int pipe_fd[2], t_req *req, void (*old_sigint)(int))
{
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		req->exit_stat = 1;
		signal(SIGINT, old_sigint);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		req->exit_stat = 1;
		signal(SIGINT, old_sigint);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sigint_handler);
		do_heredoc_child(delimiter, pipe_fd, req);
	}
	return (pid);
}

static int	check_heredoc_status(int status, int pipe_fd[2], t_req *req)
{
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		req->exit_stat = 130;
		req->heredoc_interrupted = 1;
		close(pipe_fd[0]);
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		req->exit_stat = WEXITSTATUS(status);
		close(pipe_fd[0]);
		return (-1);
	}
	return (0);
}

int	handle_heredoc(const char *delimiter, t_req *req)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	void	(*old_sigint)(int);

	if (req && req->heredoc_interrupted)
		return (-1);
	old_sigint = signal(SIGINT, SIG_IGN);
	pid = start_heredoc_process(delimiter, pipe_fd, req, old_sigint);
	if (pid == -1)
		return (-1);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, old_sigint);
	if (check_heredoc_status(status, pipe_fd, req) == -1)
		return (-1);
	return (pipe_fd[0]);
}