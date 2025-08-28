/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:38:48 by makboga           #+#    #+#             */
/*   Updated: 2025/08/28 13:50:44 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_pwd(t_shell *shell, char **args)
{
	char	*pwd;
	char	*cwd;

	(void)args;
	pwd = mini_getenv("PWD", shell->envp);
	if (pwd && pwd[0] == '/')
	{
		write(1, pwd, ft_strlen(pwd));
		write(1, "\n", 1);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (write(2, "pwd: failed to get current directory\n", 37), 1);
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}
