/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:30:00 by makboga           #+#    #+#             */
/*   Updated: 2025/07/30 16:00:05 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Yeni redirection oluştur
t_redirect *create_redirect(t_redirect_type type, char *filename)
{
    t_redirect *redirect;

    redirect = malloc(sizeof(t_redirect));
    if (!redirect)
        return (NULL);
    redirect->type = type;
    redirect->filename = ft_strdup(filename);
    redirect->fd = -1;
    redirect->next = NULL;
    return (redirect);
}

// Command'a redirection ekle
void add_redirect(t_command *cmd, t_redirect *redirect)
{
    t_redirect *current;

    if (!cmd || !redirect)
        return;
    if (!cmd->redirections)
    {
        cmd->redirections = redirect;
        return;
    }
    current = cmd->redirections;
    while (current->next)
        current = current->next;
    current->next = redirect;
}

// Redirectionları free et
void free_redirections(t_redirect *redirections)
{
    t_redirect *current;
    t_redirect *next;

    current = redirections;
    while (current)
    {
        next = current->next;
        if (current->filename)
            free(current->filename);
        if (current->fd > 2)
            close(current->fd);
        free(current);
        current = next;
    }
}

// Token redirection operatörü mü kontrol et
int is_redirect_token(char *token)
{
    if (!token)
        return (0);
    if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0 ||
        ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
        return (1);
    return (0);
}

// Redirection türünü belirle
t_redirect_type get_redirect_type(char *token)
{
    if (!token)
        return (REDIRECT_NONE);
    if (ft_strcmp(token, "<") == 0)
        return (REDIRECT_IN);
    if (ft_strcmp(token, ">") == 0)
        return (REDIRECT_OUT);
    if (ft_strcmp(token, ">>") == 0)
        return (REDIRECT_APPEND);
    if (ft_strcmp(token, "<<") == 0)
        return (REDIRECT_HEREDOC);
    return (REDIRECT_NONE);
}

// Heredoc işle
int handle_heredoc(char *delimiter)
{
    char *line;
    char *temp_filename;
    int fd;

    // Heredoc için temporary file oluştur
    temp_filename = ft_strdup("/tmp/minishell_heredoc_XXXXXX");
    fd = mkstemp(temp_filename);
    if (fd == -1)
    {
        perror("heredoc");
        free(temp_filename);
        return (-1);
    }

    // Kullanıcıdan input al
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    
    close(fd);
    fd = open(temp_filename, O_RDONLY);
    unlink(temp_filename); // Temp file'ı sil
    free(temp_filename);
    return (fd);
}

// Redirectionları setup et
int setup_redirections(t_command *cmd)
{
    t_redirect *current;
    int fd;

    if (!cmd || !cmd->redirections)
        return (0);
    
    current = cmd->redirections;
    while (current)
    {
        if (current->type == REDIRECT_IN)
        {
            fd = open(current->filename, O_RDONLY);
            if (fd == -1)
            {
                perror(current->filename);
                return (-1);
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return (-1);
            }
            close(fd);
        }
        else if (current->type == REDIRECT_OUT)
        {
            fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror(current->filename);
                return (-1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return (-1);
            }
            close(fd);
        }
        else if (current->type == REDIRECT_APPEND)
        {
            fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror(current->filename);
                return (-1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return (-1);
            }
            close(fd);
        }
        else if (current->type == REDIRECT_HEREDOC)
        {
            fd = handle_heredoc(current->filename); // filename heredoc'da delimiter
            if (fd == -1)
                return (-1);
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return (-1);
            }
            close(fd);
        }
        current = current->next;
    }
    return (0);
}
