/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:42:01 by makboga           #+#    #+#             */
/*   Updated: 2025/08/25 14:42:01 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*expand_token_var(char *line, int *i, t_req *req)
{
	char	*var_name;
	char	*var_value;
	int		start;
	int		len;
	char	**envp;

	start = *i;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (ft_strdup("$"));
	var_name = ft_substr(line, start, len);
	if (!var_name)
		return (ft_strdup(""));
	if (req->shell && req->shell->envp)
		envp = req->shell->envp;
	else
		envp = NULL;
	var_value = mini_getenv(var_name, envp);
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	close(STDIN_FILENO);
	exit(130);
}

char	*expand_line(char *line, t_req *req, int i, char *expanded)
{
	char	*tmp;
	char	*new_exp;
	char	c[2];

	while (line[i])
	{
		if (line[i] == '$')
		{
			tmp = expand_token_var(line, &i, req);
			new_exp = ft_strjoin(expanded, tmp);
			free(expanded);
			free(tmp);
			expanded = new_exp;
		}
		else
		{
			c[0] = line[i++];
			c[1] = '\0';
			new_exp = ft_strjoin(expanded, c);
			free(expanded);
			expanded = new_exp;
		}
	}
	return (expanded);
}
