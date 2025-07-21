/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:09:35 by mdalkili          #+#    #+#             */
/*   Updated: 2025/07/21 20:32:04 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void print_status(t_shell *shell)
{
	t_command *cmd = shell->command_p;
	t_parameters *param;

	while (cmd)
	{
		if (cmd->command)
			printf("komut: %s\n", cmd->command);
		else
			printf("komut: NULL\n");

		param = cmd->parameters_p;
		int i = 0;
		while (param)
		{
			if (param->parameter)
				printf("arg[%d]: %s\n", i, param->parameter);
			else
				printf("arg[%d]: NULL\n", i);
			param = param->next;
			i++;
		}
		cmd = cmd->next;
	}
}

void	parse_prompt(t_shell *shell)
{
	char	**args;
	int		i;
	t_command	*cmd;
	t_parameters	*param;

	args = ft_split(shell->prompt, ' '); // boşluklara göre ayır
	if (!args)
		return;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		exit(1);
	cmd->command = NULL;
	cmd->parameters_p = NULL;
	cmd->next = NULL;
	shell->command_p = cmd;
	i = 0;
	while (args[i])
	{
		if (i == 0) // İlk kelimeyi komut olarak al
			cmd->command = ft_strdup(args[i]);
		else
		{
			param = malloc(sizeof(t_parameters));
			if (!param)
				exit(1);
			param->parameter = ft_strdup(args[i]);
			param->next = NULL;
			if (!cmd->parameters_p)
				cmd->parameters_p = param; // Parametreyi ekle
			else
			{
				t_parameters *iter = cmd->parameters_p;
				while (iter->next)
					iter = iter->next;
				iter->next = param;
			}
		}
		i++;
	}
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
	print_status(shell);
}

void	get_hostname(t_shell *shell)
{
	t_read_file	*host_f;

	shell->hostname = NULL;
	host_f = malloc(sizeof(t_read_file));
	host_f->fd = open_file("/etc/hostname", O_RDONLY);
	host_f->b_read = 1;
	host_f->total_b_read = 0;
	while (host_f->b_read > 0)
	{
		host_f->b_read = read(host_f->fd, host_f->buffer, BUFFER_SIZE);
		host_f->result = malloc(host_f->total_b_read + host_f->b_read + 1);
		if (shell->hostname)
		{
			ft_memcpy(host_f->result, shell->hostname, host_f->total_b_read);
			free(shell->hostname);
		}
		ft_memcpy(host_f->result + host_f->total_b_read,
			host_f->buffer, host_f->b_read);
		host_f->total_b_read += host_f->b_read;
		host_f->result[host_f->total_b_read] = '\0';
		shell->hostname = host_f->result;
	}
	close(host_f->fd);
	shell->hostname[host_f->total_b_read - 1] = '\0';
	free(host_f);
}

void	get_display_info(t_shell *shell)
{
	char	*path;
	char	*properties;
	
	path = ft_strstr(shell->current_dir, getenv("USER"));
	if (!path)
		path = ft_strdup("/");
	else if (ft_strcmp(path, getenv("USER")) == 0)
		path = ft_strdup("~");
	else
		path = ft_strjoin("~", path + ft_strlen(getenv("USER")));
	properties = string_concatation((char *[]){BBLUE,
							getenv("USER"), "@", shell->hostname,":", BMAGENTA, path, RESET, "$ ", NULL});
	shell->display_info = set_and_free(shell->display_info, properties);
	free(path);
}

void	get_prompt(t_shell *shell)
{
	shell->prompt = set_and_free(shell->prompt, readline(shell->display_info));
	if(shell->prompt && *shell->prompt)
	{
		add_history(shell->prompt);
		parse_prompt(shell);
	}		
}
