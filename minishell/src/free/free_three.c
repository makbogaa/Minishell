/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_three.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:59:38 by makboga           #+#    #+#             */
/*   Updated: 2025/08/17 17:00:29 by makboga          ###   ########.fr       */
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
	free(quote->current_parameter);
	free_multiple_input(quote->parameters);
	free(quote);
}

void	free_parameters(t_parameters *params)
{
	t_parameters	*temp;

	while (params)
	{
		temp = params;
		params = params->next;
		free(temp->parameter);
		free(temp);
	}
}
