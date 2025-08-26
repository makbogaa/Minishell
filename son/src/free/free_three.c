/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_three.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haloztur <haloztur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:59:38 by makboga           #+#    #+#             */
/*   Updated: 2025/08/26 22:32:59 by haloztur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_redirections(t_redirect *redirections)
{
	t_redirect	*current;
	t_redirect	*next;

	if (!redirections)
		return ;
	current = redirections;
	while (current)
	{
		next = current->next;
		if (current->filename)
			free(current->filename);
		if (current->fd > 2)
			close(current->fd);
		free(current);
		current = next;
	}
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (shell->prompt)
		free(shell->prompt);
	if (shell->current_dir)
		free(shell->current_dir);
	if (shell->display_info)
		free(shell->display_info);
	if (shell->hostname)
		free(shell->hostname);
	free_shell_arrays(shell);
	free_command(shell);
	clear_history();
	rl_clear_history();
	rl_on_new_line();
	rl_replace_line("", 0);
}
