/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 07:09:35 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/29 03:22:11 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	handle_builtin_append(t_shell *shell, char *str, t_command **temp)
{
	char	*temp_str;

	temp_str = ft_strdup(str);
	append_command(shell, temp_str,
		prompt_type_control_loop(shell->builtin, 1, str), temp);
	free(temp_str);
}

static void	handle_token_command(t_shell *shell, char *str,
	t_command **temp, int *command)
{
	if (!*command)
	{
		if (ft_strcmp(str, "<") == 0 || ft_strcmp(str, "<<") == 0)
		{
			(append_command(shell, "cat", 0, temp), *command = 1);
			if (*temp)
				append_parameter(temp, str);
			return ;
		}
		else if (ft_strcmp(str, ">") == 0 || ft_strcmp(str, ">>") == 0)
		{
			(append_command(shell, "cat", 0, temp), *command = 1);
			if (*temp)
				append_parameter(temp, str);
			return ;
		}
		else
			(append_command(shell, "", 0, temp), *command = 1);
	}
	if (*temp)
		append_parameter(temp, str);
}

void	append(t_shell *shell, char *str, int *command, t_command **temp)
{
	int	result;

	if (!str || !shell)
		return ;
	if (!*command && str[0] == '\0')
		return ;
	result = prompt_type_control_loop(shell->builtin, 1, str);
	if (prompt_type_control_loop(shell->tokens, 0, str) == 4)
		handle_token_command(shell, str, temp, command);
	else if (!*command && (str[0] == '>' || str[0] == '<'))
	{
		append_command(shell, "cat", 0, temp);
		*command = 1;
		append_parameter(temp, str);
	}
	else if (!*command && (result == 1 || result == 2 || result == 3))
	{
		handle_builtin_append(shell, str, temp);
		*command = 1;
	}
	else if (prompt_type_control_loop(shell->tokens, 0, str))
		handle_token_command(shell, str, temp, command);
	else
		append_parameter(temp, str);
}

void	parse_prompt(t_shell *shell)
{
	t_parse	parse;

	reset_parse(&parse, shell);
	while (parse.temp_prompt && *parse.temp_prompt)
	{
		if (*parse.temp_prompt == '|')
		{
			if (handle_pipe_token(shell, &parse))
				return ;
			continue ;
		}
		set_parse_function(&parse);
		if (parse.parse_func)
		{
			if (run_parse(&parse, shell))
				break ;
			continue ;
		}
		parse.temp_prompt++;
	}
	if (parse.had_pipe && parse.command == 0)
		return (pipe_syntax_error(shell, &parse), (void)0);
	shell->command_p = parse.command_temp_p;
	(process_redirections(shell), free(parse.start));
}
