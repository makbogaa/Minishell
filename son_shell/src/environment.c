/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 16:12:49 by makboga           #+#    #+#             */
/*   Updated: 2025/08/25 16:30:14 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*mini_getenv(const char *key, char **envp)
{
	int		i;
	int		len;

	if (!key || !envp)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*create_env_entry(const char *key, const char *value)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, value);
	free(temp);
	return (result);
}

char	**mini_setenv(char **envp, const char *key, const char *value,
	int overwrite)
{
	int		i;
	int		len;
	char	*new_var;

	len = ft_strlen(key);
	i = 0;
	while (envp && envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
		{
			if (!overwrite)
				return (envp);
			free(envp[i]);
			envp[i] = create_env_entry(key, value);
			return (envp);
		}
		i++;
	}
	new_var = create_env_entry(key, value);
	return (ft_double_extension(envp, new_var));
}

char	**mini_unsetenv(char *key, char **envp)
{
	int		i;
	int		j;
	int		len;
	char	**new_env;

	len = ft_strlen(key);
	i = 0;
	j = 0;
	while (envp && envp[i])
		i++;
	new_env = malloc(sizeof(char *) * i);
	i = 0;
	while (envp && envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			free(envp[i]);
		else
			new_env[j++] = envp[i];
		i++;
	}
	new_env[j] = NULL;
	free(envp);
	return (new_env);
}

void	increment_shlvl(t_shell *shell)
{
	char	*current_shlvl;
	int		shlvl_value;
	char	*new_shlvl;
	char	*shlvl_str;

	if (!shell || !shell->envp)
		return ;
	current_shlvl = mini_getenv("SHLVL", shell->envp);
	if (current_shlvl)
		shlvl_value = ft_atoi(current_shlvl) + 1;
	else
		shlvl_value = 1;
	new_shlvl = ft_itoa(shlvl_value);
	if (!new_shlvl)
		return ;
	shlvl_str = ft_strjoin("SHLVL=", new_shlvl);
	free(new_shlvl);
	if (!shlvl_str)
		return ;
	export_single_var(&(shell->envp), shlvl_str);
	free(shlvl_str);
}
