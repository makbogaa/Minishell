/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 05:38:17 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/29 16:47:36 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_export_var_with_value(char *clean_var, int fd)
{
	char	*equal_pos;
	char	*var_name;
	char	*var_value;

	equal_pos = ft_strchr(clean_var, '=');
	var_name = ft_substr(clean_var, 0, equal_pos - clean_var);
	var_value = ft_strdup(equal_pos + 1);
	if (!ft_strcmp(var_value, "__EXPORT_ONLY__"))
	{
		ft_putendl_fd(var_name, fd);
		free(var_name);
		free(var_value);
		return ;
	}
	ft_putstr_fd(var_name, fd);
	ft_putstr_fd("=\"", fd);
	ft_putstr_fd(var_value, fd);
	ft_putendl_fd("\"", fd);
	free(var_name);
	free(var_value);
}

static void	print_marked_export_entry(char *entry, int fd)
{
	char	*export_only_pos;
	char	*clean_var;

	export_only_pos = ft_strstr(entry, "=__EXPORT_ONLY__");
	if (!export_only_pos)
	{
		print_export_var_with_value(entry, fd);
		return ;
	}
	clean_var = ft_substr(entry, 0, export_only_pos - entry);
	if (ft_strchr(clean_var, '='))
		print_export_var_with_value(clean_var, fd);
	else
		ft_putendl_fd(clean_var, fd);
	free(clean_var);
}

static void	print_unmarked_export_entry(char *entry, int fd)
{
	char	*equal_pos;
	char	*var_name;
	char	*var_value;

	equal_pos = ft_strchr(entry, '=');
	if (!equal_pos)
	{
		ft_putendl_fd(entry, fd);
		return ;
	}
	var_name = ft_substr(entry, 0, equal_pos - entry);
	var_value = ft_strdup(equal_pos + 1);
	if (!ft_strcmp(var_value, "__EXPORT_ONLY__"))
	{
		ft_putendl_fd(var_name, fd);
		free(var_name);
		free(var_value);
		return ;
	}
	ft_putstr_fd(var_name, fd);
	ft_putstr_fd("=\"", fd);
	ft_putstr_fd(var_value, fd);
	ft_putendl_fd("\"", fd);
	free(var_name);
	free(var_value);
}

static void	print_export_entry(char *entry, int fd)
{
	if (ft_strstr(entry, "=__EXPORT_ONLY__"))
		print_marked_export_entry(entry, fd);
	else
		print_unmarked_export_entry(entry, fd);
}

void	print_export_list(char **export_arr, int fd)
{
	int	i;

	i = 0;
	if (!export_arr)
		return ;
	while (export_arr[i])
	{
		ft_putstr_fd("declare -x ", fd);
		print_export_entry(export_arr[i], fd);
		i++;
	}
}
