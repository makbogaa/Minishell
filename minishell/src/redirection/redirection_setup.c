/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_setup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:00:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 12:37:39 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	create_temp_file(char **temp_filename)
{
	int	fd;

	*temp_filename = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
	fd = mkstemp(*temp_filename);
	if (fd == -1)
	{
		perror("heredoc");
		free(*temp_filename);
		return (-1);
	}
	return (fd);
}

static void	read_heredoc_input(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	handle_heredoc(char *delimiter)
{
	char	*temp_filename;
	int		fd;

	fd = create_temp_file(&temp_filename);
	if (fd == -1)
		return (-1);
	read_heredoc_input(fd, delimiter);
	close(fd);
	fd = open(temp_filename, O_RDONLY);
	unlink(temp_filename);
	free(temp_filename);
	return (fd);
}

int	process_input_redirections(t_redirect *current, int *input_fd,
								int *output_fd)
{
	if (setup_input_redirect(current, input_fd) == -1)
	{
		if (*output_fd != -1)
			close(*output_fd);
		return (-1);
	}
	return (0);
}

int	process_output_redirections(t_redirect *current, int *input_fd,
								int *output_fd)
{
	if (setup_output_redirect(current, output_fd) == -1)
	{
		if (*input_fd != -1)
			close(*input_fd);
		return (-1);
	}
	return (0);
}
