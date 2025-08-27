/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 13:40:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/25 16:18:42 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*create_env_entry(const char *key, const char *value)
{
	char	*temp;
	char	*new_entry;

	temp = ft_strjoin(key, "=");
	if (!temp)
		return (NULL);
	new_entry = ft_strjoin(temp, value);
	free(temp);
	return (new_entry);
}

static int	update_env_var(char ***envp, const char *key, const char *value)
{
	int		i;
	int		key_len;
	char	*new_entry;

	if (!envp || !*envp || !key || !value)
		return (0);
	i = 0;
	key_len = ft_strlen(key);
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], key, key_len) && (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			new_entry = create_env_entry(key, value);
			if (!new_entry)
				return (0);
			(*envp)[i] = new_entry;
			return (1);
		}
		i++;
	}
	return (0);
}

static void	add_env_var(char ***envp, const char *key_value)
{
	int		i;
	int		size;
	char	**new_envp;

	if (!envp || !*envp || !key_value)
		return ;
	i = 0;
	size = 0;
	while ((*envp)[size])
		size++;
	new_envp = malloc(sizeof(char *) * (size + 2));
	if (!new_envp)
		return ;
	while (i < size)
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	new_envp[i++] = ft_strdup(key_value);
	new_envp[i] = NULL;
	free(*envp);
	*envp = new_envp;
}

int	export_single_var(char ***envp, char *arg)
{
	char	*equal_pos;
	char	*key;

	if (!envp || !*envp || !arg)
		return (1);
	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		key = ft_substr(arg, 0, equal_pos - arg);
		if (!key)
			return (1);
		if (!update_env_var(envp, key, equal_pos + 1))
			add_env_var(envp, arg);
		free(key);
	}
	else
		add_env_var(envp, arg);
	return (0);
}
