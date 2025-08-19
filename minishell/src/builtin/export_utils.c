/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 13:40:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 13:22:03 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	update_env_var(char ***envp, const char *key, const char *value)
{
	int		i;
	int		key_len;
	char	*new_entry;
	char	*temp;

	i = 0;
	key_len = ft_strlen(key);
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], key, key_len) && (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			temp = ft_strjoin(key, "=");
			new_entry = ft_strjoin(temp, value);
			free(temp);
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
