/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:07 by makboga           #+#    #+#             */
/*   Updated: 2025/08/12 16:11:15 by makboga          ###   ########.fr       */
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
#include <fcntl.h>

# ifndef BUFFER_SIZE
# define BUFFER_SIZE	1024
# endif

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define BRED "\033[1;31m"
# define BGREEN "\033[1;32m"
# define BYELLOW "\033[1;33m"
# define BBLUE "\033[1;34m"
# define BMAGENTA "\033[1;35m"
# define RESET "\033[0m"

typedef struct s_parameters
{
	char *parameter;
	struct s_parameters *next;
}t_parameters;

// Redirection türleri
typedef enum e_redirect_type
{
	REDIRECT_NONE,
	REDIRECT_IN,        // <
	REDIRECT_OUT,       // >
	REDIRECT_APPEND,    // >>
	REDIRECT_HEREDOC    // <<
} t_redirect_type;

// Redirection yapısı
typedef struct s_redirect
{
	t_redirect_type type;
	char *filename;
	int fd;
	struct s_redirect *next;
} t_redirect;

typedef struct s_command
{
	char *command;
	struct s_parameters *parameters_p;
	char *token;
	int token_flag;
	int flag;
	int builtin;
	t_redirect *redirections;    // Yeni: redirection listesi
	struct s_command *next;
}	t_command;

// Shell yapıları
typedef struct s_shell
{
	char	*builtin[8];
	char	*tokens[8];// && || | > >> << <
	char	*prompt;
	char	**envp;
	char	*current_dir;
	char	*hostname;
	char    *display_info;
	char 	**options;
	int		last_exit_code;
	t_command *command_p;
	// İleride komut yapısı vb. ekleyebiliriz
}	t_shell;

typedef struct s_quote
{
	char	**parameters;
	char	*current_parameter;
	int		flag;
	int		len;
}t_quote;

typedef struct s_read_file_info
{
	int		fd;
	char	buffer[BUFFER_SIZE + 1];
	int		b_read;
	int		total_b_read;
	char	*result;
}t_read_file;

void	init_shell(t_shell *shell, char **envp);
void	start_minishell(t_shell *shell);
void	get_display_info(t_shell *shell);
void	get_hostname(t_shell *shell);
int		open_file(char *filename, int flags);


//PARSER
int		get_prompt(t_shell *shell);
void 	parse_prompt(t_shell *shell);
char	*single_quote_control(char **prompt,t_shell *shell);
char	*double_quote_control(char **prompt,t_shell *shell);
t_quote *quote_init(void);
int		counter_quote(char *str, char *quoter);
char 	**copy_multiple_input(char **multiple_input,char *temp, int len);
char 	*get_characters(char **prompt,t_shell *shell);
char 	*get_redirect_operator(char **prompt,t_shell *shell);
char 	*expand_if_dollar(const char *str, int *i,t_shell *shell);
char 	*get_next_char(const char *str, int *i);
char	*string_concatation_heap(char **str);
void 	append_command(t_shell *shell, char *str,int builtin, t_command **temp);
void 	append_parameter(t_parameters *new_param, t_command **temp);
void	append_token(char *str, t_command **temp);
int 	prompt_type_control_loop(char **control_list,int type,char *str);
int		is_whitespace(char c);

//EXECUTE
void 	execute(t_shell *shell);
void 	execute_single_command(t_shell *shell);
void 	handle_command_execution(t_shell *shell, char **params);
int 	setup_and_restore_redirections(t_shell *shell, int *original_stdin, int *original_stdout, int setup_mode);
int 	run(t_command *command,char **params,t_shell *shell);
void 	execute_commands(t_shell *shell, char **commands, int n);
char 	*strip_path(char *cmd);
char 	*get_path(char *cmd, char **envp);
int		has_slash(const char *s);

//FREE
void 	free_argv(char **argv);
void	free_shell(t_shell *shell);
void	ft_free_split(char **split);
void	ft_free_matrix(char **matrix);
char	*ft_strjoin_free(char *s1, const char *s2);
char	*set_and_free(char *dest, char *src);
void	free_options(t_shell *shell);
void	free_quote(t_quote *quote);
void	free_multiple_input(char **multiple_input);
void	free_command(t_shell *shell);

//BUİLTİN
int		builtin(t_command **command);
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



//UTILS
char	*string_concatation(char **str);

// REDIRECTION
t_redirect	*create_redirect(t_redirect_type type, char *filename);
void		add_redirect(t_command *cmd, t_redirect *redirect);
void		free_redirections(t_redirect *redirections);
int			setup_redirections(t_command *cmd);
int			restore_redirections(t_command *cmd);
int			handle_heredoc(char *delimiter);
int			is_redirect_token(char *token);
t_redirect_type	get_redirect_type(char *token);
void		process_redirections(t_shell *shell);
void		remove_parameter(t_parameters **head, t_parameters *to_remove);
char		*parse_redirect_token(char **prompt);

//ERROR
void exit_with_error(char *msg);
#endif
