/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_three.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haloztur <haloztur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:59:38 by makboga           #+#    #+#             */
/*   Updated: 2025/08/22 22:29:25 by haloztur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_options(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->options)
	{
		while (shell->options[i])
		{
			free(shell->options[i]);
			i++;
		}
		free(shell->options);
	}
}

void	free_quote(t_quote *quote)
{
	if (!quote)
		return ;
	free(quote->current_content);
	free_multiple_input(quote->contents);
	free(quote);
}

void	free_contents(t_list *params)
{
	t_list	*temp;

	while (params)
	{
		temp = params;
		params = params->next;
		free(temp->content);
		free(temp);
	}
}
