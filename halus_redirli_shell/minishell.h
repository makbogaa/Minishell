/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:10:07 by makboga           #+#    #+#             */
/*   Updated: 2025/08/25 16:35:48 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h> 
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_pipe_info
{
	int	*pipefds;
	int	n;
	int	i;
}t_pipe_info;

typedef enum e_redirect_type
{
	REDIRECT_NONE,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC
}t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				*filename;
	int					fd;
	struct s_redirect	*next;
}t_redirect;

typedef struct s_req
{
	int				exit_stat;
	int				heredoc_interrupted;
	struct s_shell	*shell;
}t_req;

typedef struct s_command
{
	char				*command;
	t_list				*contents_p;
	char				*token;
	int					token_flag;
	int					flag;
	int					builtin;
	t_redirect			*redirections;
	int					infile;
	int					outfile;
	struct s_command	*next;
}t_command;

typedef struct s_pipeline_data
{
	t_command	*current_cmd;
	t_req		*req;
}t_pipeline_data;

typedef struct s_shell
{
	char		*builtin[8];
	char		*tokens[8];
	char		*prompt;
	char		**envp;
	char		*current_dir;
	char		*hostname;
	char		*display_info;
	int			last_exit_code;
	t_command	*command_p;
}t_shell;

typedef struct s_quote
{
	char	**contents;
	char	*current_content;
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

void			init_shell(t_shell *shell, char **envp);
void			start_minishell(t_shell *shell);
void			get_display_info(t_shell *shell);
void			get_hostname(t_shell *shell);
int				open_file(char *filename, int flags);
void			free_shell_arrays(t_shell *shell);
int				get_prompt(t_shell *shell);
void			parse_prompt(t_shell *shell);
char			*single_quote_control(char **prompt, t_shell *shell);
char			*single_quote(char **prompt);
char			*double_quote_control(char **prompt, t_shell *shell);
char			*double_quote(char **prompt, t_shell *shell);
t_quote			*quote_init(void);
int				counter_quote(char *str, char *quoter);
char			**copy_multiple_input(char **multiple_input, char *temp,
					int len);
char			*get_characters(char **prompt, t_shell *shell);
char			*get_redirect_operator(char **prompt, t_shell *shell);
char			*expand_if_dollar(const char *str, int *i, t_shell *shell);
char			*get_next_char(const char *str, int *i);
char			*string_concatation_heap(char **str);
void			append_command(t_shell *shell, char *str, int builtin,
					t_command **temp);
void			append_content(t_list *new_param, t_command **temp);
void			append_token(char *str, t_command **temp);
int				prompt_type_control_loop(char **control_list, int type,
					char *str);
int				is_whitespace(char c);

void			execute(t_shell *shell);
void			execute_single_command(t_shell *shell);
int				handle_command_execution(t_shell *shell, char **params);
int				run(t_command *command, char **params, t_shell *shell);
void			execute_commands(t_shell *shell, char **commands, int n);
int				has_slash(const char *s);
void			close_and_free_pipes(int *pipefds, int n);

void			free_argv(char **argv);
void			free_shell(t_shell *shell);
void			ft_free_split(char **split);
void			ft_free_matrix(char **matrix);
char			*ft_strjoin_free(char *s1, const char *s2);
char			*set_and_free(char *dest, char *src);
void			free_quote(t_quote *quote);
void			free_multiple_input(char **multiple_input);
void			free_command(t_shell *shell);

int				builtin_echo(char **argv);
int				builtin_cd(t_shell *shell, char **args);
int				update_pwd_vars(t_shell *shell, char *oldpwd);
char			*get_target_directory(t_shell *shell, char **args,
					int *should_free);
int				builtin_pwd(char **args);
int				builtin_env(char **params, char **envp);
int				builtin_export(t_shell *shell, char **args);
int				builtin_unset(t_shell *shell, char *name);
int				builtin_exit(t_shell *shell, char **argv, int last_exit_code);

int				export_single_var(char ***envp, char *arg);
void			print_export_list(char **export_arr, int fd);
void			sort_export_list(char **export_arr);
int				count_env_vars(char **envp);
char			**create_export_array(char **envp);

char			*mini_getenv(const char *key, char **envp);
char			**mini_setenv(char **envp, const char *key,
					const char *value, int overwrite);
char			**ft_double_extension(char **matrix, char *new_str);
void			increment_shlvl(t_shell *shell);

void			execute(t_shell *shell);
void			execute_main(t_shell *shell);
void			handle_pipe_commands(t_shell *shell);
int				has_pipe_outside_quotes(char *str);
void			fork_and_execute(t_shell *shell, char **commands, int n,
					int *pipefds);

char			*get_path(char *cmd, char **envp);
void			execute_commands(t_shell *shell, char **commands, int n);
void			cleanup_and_wait(int *pipefds, int n, pid_t last_pid,
					t_shell *shell);
void			execute_child_process(t_shell *shell, char *command,
					t_pipe_info pipe_info);

int				handle_builtin_command(t_shell *shell, char **params,
					char *cmd_name);
char			**get_params(t_command *command);
int				handle_command_execution(t_shell *shell, char **params);
void			handle_single_command_exec(t_shell *shell);
void			execute_single_command(t_shell *shell);

char			*strip_path(char *cmd);
int				create_pipes(int **pipefds, int n);
void			setup_child_pipes(int *pipefds, int n, int i);
int				check_pipe_syntax(char *str);
int				count_list(t_command *command);
void			fill_params_array(char **params, t_command *command, int count);
char			*string_concatation(char **str);

void			free_redirections(t_redirect *redirections);
int				setup_redirections(t_command *cmd, t_shell *shell);
void			process_redirections(t_shell *shell);
void			exit_with_error(char *msg);

int				apply_redirects(t_pipeline_data *data);
void			heredoc_sigint_handler(int sig);
char			*expand_line(char *line, t_req *req, int i, char *expanded);
void			handle_heredoc_line(char *line, const char *delimiter,
					int pipe_fd[2], t_req *req);
void			free_cmd(t_command *cmd);
void			free_req(t_req *req);
int				handle_heredoc(const char *delimiter, t_req *req);

void			rm_rdr_tkns(t_list **content, t_list *prev, t_command *cmd);
t_redirect		*create_rdr_node(t_redirect_type type, char *filename);
void			cleanup_file_desc(int *last_in, int *last_out);
int				handle_output_rdr(t_redirect *redir, int *last_out, t_req *req);
int				open_redirect_file(char *filename, t_redirect_type type,
					t_req *req, int fd);
void			process_heredoc_line(char *line, int pipe_fd, t_req *req);

#endif
