/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:53:24 by makboga           #+#    #+#             */
/*   Updated: 2025/08/19 13:25:33 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_export_list(char **export_arr, int fd)
{
	int	i;

	i = 0;
	if (!export_arr)
		return ;
	while (export_arr[i])
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putendl_fd(export_arr[i++], fd);
	}
}

void	sort_export_list(char **export_arr)
{
	size_t	i;
	char	*temp;

	i = 0;
	while (export_arr[i + 1])
	{
		if (ft_strncmp(export_arr[i], export_arr[i + 1],
				ft_strlen(export_arr[i])) > 0)
		{
			temp = export_arr[i];
			export_arr[i] = export_arr[i + 1];
			export_arr[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
}

void	ft_export_list(t_shell *shell)
{
	char	**export_arr;
	int		count;
	int		i;

	count = 0;
	while (shell->envp[count])
		count++;
	export_arr = ft_calloc(count + 1, sizeof(char *));
	if (!export_arr)
		return ;
	i = 0;
	while (shell->envp[i])
	{
		export_arr[i] = ft_strdup(shell->envp[i]);
		if (!export_arr[i])
		{
			ft_free_matrix(export_arr);
			return ;
		}
		i++;
	}
	sort_export_list(export_arr);
	print_export_list(export_arr, STDOUT_FILENO);
	ft_free_matrix(export_arr);
}

int	ft_export_utils(char *str, t_shell *shell)
{
	size_t	i;
	int		bad;

	i = 0;
	bad = 0;
	if (str[0] == '=' || ft_isdigit(str[0]))
		bad = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			bad = 1;
		i++;
	}
	if (bad)
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd(": not a valid identifier", 2);
	}
	else
		export_single_var(&(shell->envp), str);
	return (bad);
}

int	builtin_export(t_shell *shell, char **args)
{
	size_t	i;
	int		flag;

	i = 1;
	flag = 0;
	if (!args[i])
		ft_export_list(shell);
	while (args[i])
	{
		if (ft_export_utils(args[i], shell))
			flag = 1;
		i++;
	}
	return (flag);
}
