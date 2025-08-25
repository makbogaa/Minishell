/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:24:27 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/19 15:14:55 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	find_var_end(const char *str, int start)
{
    int j;
    
    j = start;
    while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
        j++;
    return (j);
}

static char	*extract_and_expand_var(const char *str, int start, int end, t_shell *shell)
{
    char	*expand_value;
    char	*expanded;
    
    expand_value = ft_strndup(str + start, end - start);
    if (!expand_value)
        return (ft_strdup(""));
    expanded = mini_getenv(expand_value, shell->envp) ? ft_strdup(mini_getenv(expand_value, shell->envp)) : ft_strdup("");
    free(expand_value);
    return (expanded);
}

char *expand_if_dollar(const char *str, int *i,t_shell *shell)
{
    int j;
    
    j = *i + 1;
    if(str[*i + 1] == '?')
    {
        (*i)+=2;
        return ft_itoa(shell->last_exit_code);
    }
    // Bash $"..." syntax: locale-aware string (literal $ inside)
    if (str[*i + 1] == '"') {
        (*i)++;
        return ft_strdup("$");
    }
    if (!(ft_isalnum(str[j]) || str[j] == '_')) {
        (*i)++;
        return ft_strdup("$");
    }
    j = find_var_end(str, j);
    char *result = extract_and_expand_var(str, *i + 1, j, shell);
    *i = j;
    return (result);
}
char *get_next_char(const char *str, int *i)
{
    char *tmp = ft_strndup(str + *i, 1);
    (*i)++;
    return tmp;
}

char *get_characters(char **prompt, t_shell *shell)
{
    char *result = NULL;
    char *expanded = NULL;
    char *temp = NULL;
    int i = 0;
    
    // Space olmadıkça devam et ve quote'ları da handle et
    while ((*prompt)[i] && !is_whitespace((*prompt)[i]) &&
           (*prompt)[i] != '>' && (*prompt)[i] != '<' && (*prompt)[i] != '|') {
        if ((*prompt)[i] == '$') {
            // Şu ana kadar olan kısmı al
            if (i > 0) {
                temp = ft_substr(*prompt, 0, i);
                if (result) {
                    expanded = ft_strjoin(result, temp);
                    free(result);
                    free(temp);
                    result = expanded;
                } else {
                    result = temp;
                }
            }
            // $ expansion yap
            *prompt += i;
            temp = expand_if_dollar(*prompt, &i, shell);
            if (result) {
                expanded = ft_strjoin(result, temp);
                free(result);
                free(temp);
                result = expanded;
            } else {
                result = temp;
            }
            *prompt += i;
            i = 0;
        } else if ((*prompt)[i] == '\'') {
            // Şu ana kadar olan kısmı al
            if (i > 0) {
                temp = ft_substr(*prompt, 0, i);
                if (result) {
                    expanded = ft_strjoin(result, temp);
                    free(result);
                    free(temp);
                    result = expanded;
                } else {
                    result = temp;
                }
                *prompt += i;
                i = 0;
            }
            // Single quote handle et
            temp = single_quote(prompt);
            if (result) {
                expanded = ft_strjoin(result, temp);
                free(result);
                free(temp);
                result = expanded;
            } else {
                result = temp;
            }
        } else if ((*prompt)[i] == '"') {
            // Şu ana kadar olan kısmı al
            if (i > 0) {
                temp = ft_substr(*prompt, 0, i);
                if (result) {
                    expanded = ft_strjoin(result, temp);
                    free(result);
                    free(temp);
                    result = expanded;
                } else {
                    result = temp;
                }
                *prompt += i;
                i = 0;
            }
            // Double quote handle et
            temp = double_quote(prompt, shell);
            if (result) {
                expanded = ft_strjoin(result, temp);
                free(result);
                free(temp);
                result = expanded;
            } else {
                result = temp;
            }
        } else {
            i++;
        }
    }
    
    // Kalan kısmı ekle
    if (i > 0) {
        temp = ft_substr(*prompt, 0, i);
        if (result) {
            expanded = ft_strjoin(result, temp);
            free(result);
            free(temp);
            result = expanded;
        } else {
            result = temp;
        }
        *prompt += i;
    }
    
    return (result);
}

// Redirection operatörlerini parse eden fonksiyon
char *get_redirect_operator(char **prompt,t_shell *shell)
{
    char *result;
    
    (void)shell;
    if (!*prompt || !**prompt)
        return (NULL);
        
    // >> operatörü
    if (**prompt == '>' && *(*prompt + 1) == '>')
    {
        result = ft_strdup(">>");
        *prompt += 2;
        return (result);
    }
    // << operatörü  
    else if (**prompt == '<' && *(*prompt + 1) == '<')
    {
        result = ft_strdup("<<");
        *prompt += 2;
        return (result);
    }
    // > operatörü
    else if (**prompt == '>')
    {
        result = ft_strdup(">");
        *prompt += 1;
        return (result);
    }
    // < operatörü
    else if (**prompt == '<')
    {
        result = ft_strdup("<");
        *prompt += 1;
        return (result);
    }
    
    return (NULL);
}
