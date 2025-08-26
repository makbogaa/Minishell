/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:35:00 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/26 04:28:29 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	set_parse_function(t_parse *parse)
{
	parse->parse_func = NULL;
	if (*parse->temp_prompt == '\'')
		parse->parse_func = single_quote_control;
	else if (*parse->temp_prompt == '"')
		parse->parse_func = double_quote_control;
	else if (*parse->temp_prompt == '>' || *parse->temp_prompt == '<')
		parse->parse_func = get_redirect_operator;
	else if (!ft_isspace(*parse->temp_prompt))
		parse->parse_func = get_characters;
}

int	handle_pipe_token(t_shell *shell, t_parse *parse)
{
	if (parse->command == 0)
	{
		pipe_syntax_error(shell, parse);
		return (1);
	}
	parse->had_pipe = 1;
	parse->command = 0;
	parse->command_temp_p = NULL;
	parse->temp_prompt++;
	return (0);
}
