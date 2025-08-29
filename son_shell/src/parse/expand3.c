/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:17:37 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/29 04:16:19 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_valid_tilde_position(char *prompt, int i)
{
	if (i == 0)
		return (1);
	if (ft_isspace(prompt[i - 1]))
		return (1);
	return (0);
}

char	*process_character_at_index(char **prompt,
	int *i, t_shell *shell, int context)
{
	char	*tmp;
	char	*home_value;
	int		old_i;

	old_i = *i;
	if (!*prompt || !prompt || *i < 0)
		return (NULL);
	if ((*prompt)[*i] == '$' && context != 1)
		tmp = expand_if_dollar(*prompt, i, shell);
	else if ((*prompt)[*i] == '$' && context == 1)
		tmp = get_next_char(*prompt, i);
	else if ((*prompt)[*i] == '~' && is_valid_tilde_position(*prompt, *i))
	{
		*i += 1;
		home_value = mini_getenv("HOME", shell->envp);
		if (home_value)
			tmp = ft_strdup(home_value);
		else
			tmp = ft_strdup("~");
	}
	else
		tmp = get_next_char(*prompt, i);
	if (*i <= old_i)
		*i = old_i + 1;
	return (tmp);
}

char	*get_next_char(const char *str, int *i)
{
	char	*tmp;

	tmp = ft_strndup(str + *i, 1);
	(*i)++;
	return (tmp);
}

int	is_heredoc_context(t_shell *shell)
{
	t_command	*current;
	t_list		*last_param;

	if (!shell || !shell->command_p)
		return (0);
	current = shell->command_p;
	while (current->next)
		current = current->next;
	if (!current->contents_p)
		return (0);
	last_param = current->contents_p;
	while (last_param->next)
		last_param = last_param->next;
	if (last_param->content
		&& ft_strcmp((char *)last_param->content, "<<") == 0)
		return (1);
	return (0);
}
