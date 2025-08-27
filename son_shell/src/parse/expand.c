/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:24:27 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/27 19:30:17 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	find_var_end(const char *str, int start)
{
	int	j;

	j = start;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	return (j);
}

static char	*extract_and_expand_var(const char *str, int start, int end)
{
	char	*expand_value;
	char	*expanded;

	expand_value = ft_strndup(str + start, end - start);
	if (!expand_value)
		return (ft_strdup(""));
	if (getenv(expand_value))
		expanded = ft_strdup(getenv(expand_value));
	else
		expanded = ft_strdup("");
	free(expand_value);
	return (expanded);
}

static char	*append_to_result(char *result, char *tmp)
{
	char	*new_result;

	if (result)
		new_result = ft_strjoin(result, tmp);
	else
		new_result = ft_strjoin("", tmp);
	if (result)
		free(result);
	free(tmp);
	return (new_result);
}

char	*expand_if_dollar(const char *str, int *i, t_shell *shell)
{
	int		j;
	char	*result;

	j = *i + 1;
	if (str[*i + 1] == '?')
	{
		(*i) += 2;
		return (ft_itoa(shell->last_exit_code));
	}
	if (str[*i + 1] == '"')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	if (!(ft_isalnum(str[j]) || str[j] == '_'))
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	j = find_var_end(str, j);
	result = extract_and_expand_var(str, *i + 1, j);
	*i = j;
	return (result);
}

char	*get_characters(char **prompt, t_shell *shell)
{
	char	*result;
	char	*tmp;
	int		i;

	result = NULL;
	i = 0;
	while ((*prompt)[i] && !ft_isspace((*prompt)[i]) &&
		(*prompt)[i] != '\'' && (*prompt)[i] != '"' && !ft_ismeta(*prompt, i))
	{
		tmp = process_character_at_index(prompt, &i, shell);
		if (tmp)
			result = append_to_result(result, tmp);
	}
	*prompt += i;
	if(!result)
		return (ft_strdup(""));
	return (re_control(tmp, result, prompt, shell));
}
