/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 04:17:37 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/26 05:15:25 by mdalkili         ###   ########.fr       */
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

char	*process_character_at_index(char **prompt, int *i, t_shell *shell)
{
	char	*tmp;
	char	*home_value;
	int		old_i;

	old_i = *i;
	if (!*prompt || !prompt || *i < 0)
		return (NULL);
	if ((*prompt)[*i] == '$')
		tmp = expand_if_dollar(*prompt, i, shell);
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
