/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:18:59 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 15:02:07 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_env(char **params, char **envp)
{
	int	i;

	if (params && params[1])
	{
		write(STDERR_FILENO, "env: '", 6);
		write(STDERR_FILENO, params[1], ft_strlen(params[1]));
		write(STDERR_FILENO, "': No such file or directory\n", 29);
		return (127);
	}
	i = 0;
	while (envp && envp[i])
	{
		if (!ft_strstr(envp[i], "=__EXPORT_ONLY__"))
		{
			write(1, envp[i], ft_strlen(envp[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}
