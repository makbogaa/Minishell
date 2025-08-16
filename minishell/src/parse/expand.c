/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:24:27 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/16 15:27:45 by makboga          ###   ########.fr       */
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

static char	*extract_and_expand_var(const char *str, int start, int end)
{
    char	*expand_value;
    char	*expanded;
    
    expand_value = ft_strndup(str + start, end - start);
    if (!expand_value)
        return (ft_strdup(""));
    expanded = getenv(expand_value) ? ft_strdup(getenv(expand_value)) : ft_strdup("");
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
    char *result = extract_and_expand_var(str, *i + 1, j);
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
    char *tmp;
    int i = 0;
    
    // Bash mantığı: space olmadıkça devam et (quotes dahil)
    while ((*prompt)[i] && !is_whitespace((*prompt)[i]) &&
           (*prompt)[i] != '>' && (*prompt)[i] != '<' && (*prompt)[i] != '|') {
        if ((*prompt)[i] == '"') {
            // Quote'u double_quote_control ile parse et
            char *quote_start = *prompt + i;
            char *temp_prompt = quote_start;
            tmp = double_quote_control(&temp_prompt, shell);
            i = temp_prompt - *prompt;
        } else if ((*prompt)[i] == '\'') {
            // Quote'u single_quote_control ile parse et
            char *quote_start = *prompt + i;
            char *temp_prompt = quote_start;
            tmp = single_quote_control(&temp_prompt, shell);
            i = temp_prompt - *prompt;
        } else if ((*prompt)[i] == '$') {
            // Bash $"..." syntax özel durumu
            if ((*prompt)[i + 1] == '"') {
                // $"..." tüm string'ini parse et
                int quote_end = i + 2;
                while ((*prompt)[quote_end] && (*prompt)[quote_end] != '"')
                    quote_end++;
                if ((*prompt)[quote_end] == '"') {
                    // $"content" → content'i literal olarak al
                    tmp = ft_strndup(*prompt + i + 2, quote_end - i - 2);
                    i = quote_end + 1;
                } else {
                    tmp = expand_if_dollar(*prompt, &i, shell);
                }
            } else {
                tmp = expand_if_dollar(*prompt, &i, shell);
            }
        } else {
            tmp = get_next_char(*prompt, &i);
        }
        
        if (tmp) {
            char *new_result = ft_strjoin(result ? result : "", tmp);
            free(result);
            free(tmp);
            result = new_result;
        }
    }
    
    *prompt += i;
    return result ? result : ft_strdup("");
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
