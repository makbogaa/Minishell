/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 13:40:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/29 16:31:21 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*extract_var_name(const char *arg)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		return (ft_substr(arg, 0, equal_pos - arg));
	else
		return (ft_strdup(arg));
}

static int	find_export_var_index(char **envp, const char *var_name)
{
	int		i;
	char	*current_name;
	char	*equal_pos;

	if (!envp || !var_name)
		return (-1);
	i = 0;
	while (envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		if (equal_pos)
		{
			current_name = ft_substr(envp[i], 0, equal_pos - envp[i]);
			if (current_name && ft_strcmp(current_name, (char *)var_name) == 0)
			{
				free(current_name);
				return (i);
			}
			free(current_name);
		}
		i++;
	}
	return (-1);
}

static void	update_export_var(char ***envp, int index, const char *new_entry)
{
	if (!envp || !*envp || index < 0 || !new_entry)
		return ;
	free((*envp)[index]);
	(*envp)[index] = ft_strdup(new_entry);
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
	char	*var_name;
	char	*marked_arg;
	int		existing_index;

	if (!envp || !*envp || !arg)
		return (1);
	var_name = extract_var_name(arg);
	if (!var_name)
		return (1);
	if (!ft_strchr(arg, '='))
		marked_arg = ft_strjoin(arg, "=__EXPORT_ONLY__");
	else
		marked_arg = ft_strdup(arg);
	if (!marked_arg)
		return (free(var_name), 1);
	existing_index = find_export_var_index(*envp, var_name);
	if (existing_index >= 0)
		update_export_var(envp, existing_index, marked_arg);
	else
		add_env_var(envp, marked_arg);
	free(var_name);
	free(marked_arg);
	return (0);
}
