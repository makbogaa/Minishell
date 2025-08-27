/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:08:25 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 19:02:51 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_cd(t_shell *shell, char **args)
{
	char	*oldpwd;
	char	*newpwd;
	int		should_free_expanded;

	if (args[1] && args[2])
		return (write(2, "cd: too many arguments\n", 23), 1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (write(2, "cd: error getting current directory\n", 36), 1);
	newpwd = get_target_directory(shell, args, &should_free_expanded);
	if (!newpwd)
		return (free(oldpwd), 1);
	if (chdir(newpwd) != 0)
	{
		if (should_free_expanded)
			free(newpwd);
		free(oldpwd);
		return (write(2, "cd: No such file or directory\n", 30), 1);
	}
	update_pwd_vars(shell, oldpwd);
	if (should_free_expanded)
		free(newpwd);
	free(oldpwd);
	return (0);
}
