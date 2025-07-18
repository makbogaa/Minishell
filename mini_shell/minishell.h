/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:07 by makboga           #+#    #+#             */
/*   Updated: 2025/07/17 19:01:48 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

// Shell yapıları
typedef struct s_shell
{
	char	*prompt;
	char	**envp;
	char	*current_dir;
	char    *display_info;
	// İleride komut yapısı vb. ekleyebiliriz
}	t_shell;

void	init_shell(t_shell *shell, char **envp);
void	start_minishell(t_shell *shell);

//FREE
void 	free_argv(char **argv);
void	free_shell(t_shell *shell);
void	ft_free_split(char **split);
void	ft_free_matrix(char **matrix);
char	*ft_strjoin_free(char *s1, const char *s2);

//BUİLTİN
int 	builtin_exe(t_shell *shell, char **argv);
int		builtin_echo(char **argv);
int		builtin_cd(t_shell *shell, char **args);
int 	builtin_pwd(void);
int 	builtin_env(char **envp);
int		builtin_export(char ***envp, char **argv);
int		builtin_unset(t_shell *shell, char *name);
void	builtin_exit(char **argv);

//ENVIRONMENT
char	*mini_getenv(const char *key, char **envp);
char 	**mini_setenv(char **envp, const char *key, const char *value, int overwrite);
char	**mini_unsetenv(char *key, char **envp);
char	**ft_double_extension(char **matrix, char *new_str);
char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3);

void execute_commands(t_shell *shell, char **commands, int n);


#endif
