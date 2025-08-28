/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdalkili <mdalkilic344@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 19:00:34 by makboga           #+#    #+#             */
/*   Updated: 2025/08/28 05:57:08 by mdalkili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*expand_tilde_path(char *path, char **envp)
{
	char	*home;
	char	*expanded;

	if (path[0] != '~')
		return (path);
	home = mini_getenv("HOME", envp);
	if (!home)
		return (NULL);
	if (path[1] == '\0')
		return (ft_strdup(home));
	expanded = ft_strjoin(home, path + 1);
	return (expanded);
}

int	update_pwd_vars(t_shell *shell, char *oldpwd)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
		return (write(2, "cd: error getting current directory\n", 36), 1);
	shell->envp = mini_setenv(shell->envp, "OLDPWD", oldpwd, 1);
	shell->envp = mini_setenv(shell->envp, "PWD", newpwd, 1);
	free(newpwd);
	return (0);
}

static int	is_special_arg(char *arg)
{
	if (!arg || !arg[0])
		return (1);
	if (ft_strcmp(arg, "-") == 0)
		return (2);
	return (0);
}

static char	*handle_home_oldpwd(t_shell *shell, int special_type)
{
	char	*newpwd;

	if (special_type == 1)
	{
		newpwd = mini_getenv("HOME", shell->envp);
		if (!newpwd)
			return (write(2, "cd: HOME not set\n", 17), NULL);
		return (ft_strdup(newpwd));
	}
	if (special_type == 2)
	{
		newpwd = mini_getenv("OLDPWD", shell->envp);
		if (!newpwd)
			return (write(2, "cd: OLDPWD not set\n", 19), NULL);
		printf("%s\n", newpwd);
		return (ft_strdup(newpwd));
	}
	return (NULL);
}

char	*get_target_directory(t_shell *shell, char **args, int *should_free)
{
	char	*result;
	char	*expanded_path;
	int		special_type;

	*should_free = 1;
	special_type = is_special_arg(args[1]);
	if (special_type)
	{
		result = handle_home_oldpwd(shell, special_type);
		return (result);
	}
	expanded_path = expand_tilde_path(args[1], shell->envp);
	if (!expanded_path)
		return (write(2, "cd: HOME not set\n", 17), NULL);
	if (expanded_path == args[1])
		*should_free = 0;
	return (expanded_path);
}
