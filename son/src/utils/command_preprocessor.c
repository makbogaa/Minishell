/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_preprocessor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:00:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 13:39:42 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*normalize_redirections(char *command)
{
	if (!command)
		return (NULL);
	while (*command == ' ' || *command == '\t')
		command++;
	if (*command == '>' || *command == '<')
		return (ft_strdup(command));
	return (ft_strdup(command));
}

char	**ft_double_extension(char **matrix, char *new_str)
{
	int		i;
	char	**new_matrix;

	if (!new_str)
		return (matrix);
	i = 0;
	while (matrix && matrix[i])
		i++;
	new_matrix = malloc(sizeof(char *) * (i + 2));
	if (!new_matrix)
		return (NULL);
	i = 0;
	while (matrix && matrix[i])
	{
		new_matrix[i] = matrix[i];
		i++;
	}
	new_matrix[i] = new_str;
	new_matrix[i + 1] = NULL;
	free(matrix);
	return (new_matrix);
}
