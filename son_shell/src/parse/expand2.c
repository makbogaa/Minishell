/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 02:06:35 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/26 04:19:51 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*expand_tilde(const char *str, int *i, t_shell *shell)
{
	if (!ft_strncmp("~root", str, ft_strlen("~root")))
	{
		*i += ft_strlen("~root");
		return (ft_strdup("/root"));
	}
	*i += 1;
	return (ft_strdup(mini_getenv("HOME", shell->envp)));
}

static char	*handle_double_redirect(char **prompt)
{
	char	*result;

	if (**prompt == '>' && *(*prompt + 1) == '>')
	{
		result = ft_strdup(">>");
		*prompt += 2;
		return (result);
	}
	if (**prompt == '<' && *(*prompt + 1) == '<')
	{
		result = ft_strdup("<<");
		*prompt += 2;
		return (result);
	}
	return (NULL);
}

static char	*handle_single_redirect(char **prompt)
{
	char	*result;

	if (**prompt == '>')
	{
		result = ft_strdup(">");
		*prompt += 1;
		return (result);
	}
	if (**prompt == '<')
	{
		result = ft_strdup("<");
		*prompt += 1;
		return (result);
	}
	return (NULL);
}

char	*get_redirect_operator(char **prompt, t_shell *shell)
{
	char	*result;

	(void)shell;
	if (!*prompt || !**prompt)
		return (NULL);
	result = handle_double_redirect(prompt);
	if (result)
		return (result);
	return (handle_single_redirect(prompt));
}
