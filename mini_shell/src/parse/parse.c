/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:09:35 by mdalkili          #+#    #+#             */
/*   Updated: 2025/07/18 18:55:05 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void append_command(t_shell *shell, char *str, t_command **temp)
{
    int i = 0;
    int is_command = 0;

    while (shell->builtin[i])
    {
        if (ft_strcmp(shell->builtin[i], str) == 0)
        {
            is_command = 1;
            if (!shell->command_p)
            {
                shell->command_p = malloc(sizeof(t_command));
                shell->command_p->command = ft_strdup(str);
                shell->command_p->next = NULL;
                shell->command_p->parameters_p = NULL;
                *temp = shell->command_p;
            }
            else
            {
                (*temp)->next = malloc(sizeof(t_command));
                (*temp)->next->command = ft_strdup(str);
                (*temp)->next->next = NULL;
                (*temp)->next->parameters_p = NULL;
                *temp = (*temp)->next;
            }
            break; // Komut bulundu, döngüden çık
        }
        i++;
    }
    // Eğer komut değilse ve bir komut eklenmişse parametre ekle
    if (!is_command && *temp)
    {
        t_parameters *new_param = malloc(sizeof(t_parameters));
        new_param->parameter = ft_strdup(str);
        new_param->next = NULL;
        if (!(*temp)->parameters_p)
            (*temp)->parameters_p = new_param;
        else
        {
            t_parameters *p = (*temp)->parameters_p;
            while (p->next)
                p = p->next;
            p->next = new_param;
        }
    }
    if (*temp && (*temp)->command)
    {
		printf("%s\n", (*temp)->command);
		t_parameters *param = (*temp)->parameters_p;
		while (param)
    	{
			param = (*temp)->parameters_p;
			printf("%s\n",param->parameter);
			param = param->next;
    	}
	}
}


void parse_prompt(t_shell *shell)
{
	char 	**temp;
	char	*temp_prompt;
	char	*start;
	char	*current_option;
	t_command *temp_p;
	
	temp_p = NULL;
	temp_prompt = ft_strdup(shell->prompt);
	start = temp_prompt;
	temp = &temp_prompt;
	while(*temp && **temp)
	{
		if(**temp == '\'')
		{
			current_option = single_quote_control(temp);
			append_command(shell,current_option,&temp_p);
			free(current_option);
			continue;
		}
		else if(**temp == '"')
		{
			current_option = double_quote_control(temp);
			free(current_option);
			continue;
		}
		else if(**temp != ' ')
		{
			current_option = get_characters(temp);
			free(current_option);
			continue;
		}
		(*temp)++;
	}
	free(start);
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
		parse_prompt(shell);
}
