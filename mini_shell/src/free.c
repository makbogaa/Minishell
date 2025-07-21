/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:23:54 by makboga           #+#    #+#             */
/*   Updated: 2025/07/21 17:32:39 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_argv(char **argv)
{
    int j = 0;
    while (argv[j])
    {
        free(argv[j]);
        j++;
    }
    free(argv);
}

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void free_shell(t_shell *shell)
{
	int	i;

	if (!shell)
		return;
	if (shell->prompt)
		free(shell->prompt);
	if (shell->current_dir)
		free(shell->current_dir);
	if (shell->display_info)
		free(shell->display_info);
	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
		{
			free(shell->envp[i]);
			i++;
		}
		free(shell->envp);
	}
}

void	ft_free_matrix(char **matrix)
{
	int	i = 0;
	while (matrix && matrix[i])
		free(matrix[i++]);
	free(matrix);
}
char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
	{
		free(s1); // s1 zaten malloc’lı olduğu için çöpe gitmesin
		return (NULL);
	}
	ft_strlcpy(result, s1, len1 + 1);
	ft_strlcat(result, s2, len1 + len2 + 1);
	free(s1); // sadece s1’i free’liyoruz
	return (result);
}

char	*set_and_free(char *dest, char *src)
{
	if(dest)
		free(dest);
	if (!src)
		return (NULL);
	return (src);
}
void free_options(t_shell *shell)
{
	int i;

	i = 0;
	if (shell->options)
	{
		while (shell->options[i])
		{
			free(shell->options[i]);
			i++;
		}
		free(shell->options);
	}
}

void free_quote(t_quote *quote)
{
	if (!quote)
		return;
	free(quote->current_parameter);
	free_multiple_input(quote->parameters);
	free(quote);
}