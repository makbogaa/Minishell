/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:30:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/12 19:41:17 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
    int input_fd = -1;
    int output_fd = -1;
    int has_file_args = 0;

    if (!cmd || !cmd->redirections)
        return (0);
        
    // Komutun file argümanları var mı kontrol et
    if (cmd->parameters_p)
        has_file_args = 1;
    
    // İlk pass: Tüm redirection'ları kontrol et - herhangi bir hata varsa fail et
    current = cmd->redirections;
    while (current)
    {
        if (current->type == REDIRECT_IN)
        {
            // Input dosyasının var olup olmadığını kontrol et
            if (access(current->filename, F_OK) == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(current->filename, 2);
                ft_putstr_fd(": No such file or directory\n", 2);
                return (-1);
            }
            if (access(current->filename, R_OK) == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(current->filename, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                return (-1);
            }
        }
        else if (current->type == REDIRECT_OUT || current->type == REDIRECT_APPEND)
        {
            // Output dosyasını test et - açılabilir mi kontrol et
            int flags = (current->type == REDIRECT_OUT) ? 
                (O_WRONLY | O_CREAT | O_TRUNC) : (O_WRONLY | O_CREAT | O_APPEND);
            fd = open(current->filename, flags, 0644);
            if (fd == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(current->filename, 2);
                ft_putstr_fd(": Permission denied\n", 2);
                return (-1);
            }
            close(fd); // Test amaçlı, şimdilik kapat
        }
        current = current->next;
    }
    
    // İkinci pass: Gerçek redirection'ları setup et
    // Bash davranışı: tüm output dosyalarını sırayla oluştur/truncate et
    current = cmd->redirections;
    while (current)
    {
        if (current->type == REDIRECT_IN)
        {
            // Eğer komutun file argümanları varsa input redirection'ı skip et
            if (has_file_args)
            {
                current = current->next;
                continue;
            }
            
            fd = open(current->filename, O_RDONLY);
            if (fd == -1)
            {
                perror(current->filename);
                if (input_fd != -1)
                    close(input_fd);
                if (output_fd != -1)
                    close(output_fd);
                return (-1);
            }
            // Önceki input fd'yi kapat - sadece son input'u kullan
            if (input_fd != -1)
                close(input_fd);
            input_fd = fd;
        }
        else if (current->type == REDIRECT_OUT)
        {
            // Bu file'ı oluştur/truncate et - bash gibi her defasında
            fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror(current->filename);
                if (input_fd != -1)
                    close(input_fd);
                if (output_fd != -1)
                    close(output_fd);
                return (-1);
            }
            // Eğer bu son output değilse, file'ı kapat (sadece oluştur/truncate için açtık)
            // Son output ise, fd'yi sakla
            if (output_fd != -1)
                close(output_fd);
            output_fd = fd;
        }
        else if (current->type == REDIRECT_APPEND)
        {
            // Bu file'ı oluştur (append mode)
            fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror(current->filename);
                if (input_fd != -1)
                    close(input_fd);
                if (output_fd != -1)
                    close(output_fd);
                return (-1);
            }
            // Eğer bu son output değilse, file'ı kapat
            // Son output ise, fd'yi sakla
            if (output_fd != -1)
                close(output_fd);
            output_fd = fd;
        }
        else if (current->type == REDIRECT_HEREDOC)
        {
            // Eğer komutun file argümanları varsa input redirection'ı skip et
            if (has_file_args)
            {
                current = current->next;
                continue;
            }
            
            fd = handle_heredoc(current->filename); // filename heredoc'da delimiter
            if (fd == -1)
            {
                if (input_fd != -1)
                    close(input_fd);
                if (output_fd != -1)
                    close(output_fd);
                return (-1);
            }
            // Önceki input fd'yi kapat - sadece son input'u kullan
            if (input_fd != -1)
                close(input_fd);
            input_fd = fd;
        }
        current = current->next;
    }
    
    // Şimdi actual dup2'ları yap - sadece sakladığımız fd'leri kullan
    if (input_fd != -1)
    {
        if (dup2(input_fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(input_fd);
            if (output_fd != -1)
                close(output_fd);
            return (-1);
        }
        close(input_fd);
    }
    
    if (output_fd != -1)
    {
        if (dup2(output_fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            close(output_fd);
            return (-1);
        }
        close(output_fd);
    }
    
    return (0);
}
