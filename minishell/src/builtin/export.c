/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:53:24 by makboga           #+#    #+#             */
/*   Updated: 2025/07/30 18:13:43 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_valid_identifier(const char *str)
{
	int i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	update_env(char ***envp, const char *key, const char *value)
{
	int i = 0;
	int key_len = ft_strlen(key);
	char *new_entry;

	// Hem minishell environment'ını hem sistem environment'ını güncelle
	*envp = mini_setenv(*envp, key, value, 1);
	setenv(key, value, 1);
	
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], key, key_len) && (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			new_entry = ft_strjoin_3(key, "=", value);
			if (!new_entry)
				return (0);
			(*envp)[i] = new_entry;
			return (1);
		}
		i++;
	}
	return (0);
}

static void	add_env(char ***envp, const char *key_value)
{
	int i = 0;
	int size = 0;
	char **new_envp;

	while ((*envp)[size])
		size++;
	new_envp = malloc(sizeof(char *) * (size + 2));
	if (!new_envp)
		return;
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

static int	print_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static int	export_single_var_helper(char ***envp, char *key, char *arg, char *equal_pos)
{
	if (equal_pos)
	{
		// Değer var: KEY=VALUE
		if (!update_env(envp, key, equal_pos + 1))
			add_env(envp, arg);
	}
	else
	{
		// Değer yok: sadece KEY
		// Özel marker ile environment'a ekleyelim: KEY=__EXPORT_ONLY__
		// Bu marker env'de filtrelenecek ama export'ta gösterilecek
		char *export_entry = ft_strjoin(key, "=__EXPORT_ONLY__");
		if (export_entry)
		{
			add_env(envp, export_entry);
			free(export_entry);
		}
	}
	return (0);
}

static int	export_single_var(char ***envp, char *arg)
{
	char	*equal_pos;
	char	*key;
	int		ret;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		key = ft_substr(arg, 0, equal_pos - arg);
	else
		key = ft_strdup(arg);
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
	{
		free(key);
		return (print_invalid_identifier(arg));
	}
	ret = export_single_var_helper(envp, key, arg, equal_pos);
	free(key);
	return (ret);
}


int	builtin_export(char ***envp, char **argv)
{
	int	i;
	int err;
	char *marker_pos;
	char *clean_entry;

	i = 1;
	err = 0;
	if (!argv[1])
	{
		// Argüman yoksa tüm environment değişkenlerini declare -x formatında göster
		i = 0;
		while ((*envp)[i])
		{
			// __EXPORT_ONLY__ marker'ı varsa temizleyip göster
			marker_pos = ft_strstr((*envp)[i], "=__EXPORT_ONLY__");
			if (marker_pos)
			{
				// KEY=__EXPORT_ONLY__ -> declare -x KEY
				clean_entry = ft_substr((*envp)[i], 0, marker_pos - (*envp)[i]);
				printf("declare -x %s\n", clean_entry);
				free(clean_entry);
			}
			else
			{
				// Normal entry
				printf("declare -x %s\n", (*envp)[i]);
			}
			i++;
		}
		return (0);
	}
	while (argv[i])
	{
		if (export_single_var(envp, argv[i]) != 0)
			err = 1;
		i++;
	}
	return (err);
}
