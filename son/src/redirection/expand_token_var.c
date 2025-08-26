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

static char	*expand_token_var(char *line, int *i, t_req *req, int len)
{
	char	*var_name;
	char	*var_value;
	int		start;
	char	**envp;

	if (line[*i] == '$')
		(*i)++;
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
			tmp = expand_token_var(line, &i, req, 0);
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

static void	apply_file_descriptors(t_command *cmd)
{
	if (cmd->infile != -1)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (cmd->outfile != -1)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
}

int	setup_redirections(t_command *cmd, t_shell *shell)
{
	t_req			req;
	t_pipeline_data	data;

	if (!cmd || !cmd->redirections)
		return (0);
	cmd->infile = -1;
	cmd->outfile = -1;
	req.exit_stat = 0;
	req.heredoc_interrupted = 0;
	req.shell = shell;
	data.current_cmd = cmd;
	data.req = &req;
	if (apply_redirects(&data) != 0)
	{
		shell->last_exit_code = req.exit_stat;
		return (-1);
	}
	apply_file_descriptors(cmd);
	return (0);
}
