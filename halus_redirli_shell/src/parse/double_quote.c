/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haloztur <haloztur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:07:25 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/22 22:29:25 by haloztur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char *dq_expand_and_concat(const char *str, int start, int end,t_shell *shell)
{
    char *result;
    char *tmp;
    int i;
    
    result = NULL;
    i = start;
    while (i <= end ) {
		if(str[i] == '"')
		{
			i++;
			continue;
		}
        if (str[i] == '$')
            tmp = expand_if_dollar(str, &i,shell);
        else
            tmp = get_next_char(str, &i);
        char *new_result = ft_strjoin(result ? result : "", tmp);
        free(result);
        free(tmp);
        result = new_result;
    }
    return result ? result : ft_strdup("");
}

static void double_quote_loop(t_quote *quote,t_shell *shell)
{
	char *expanded;
	
	quote->current_content = set_and_free(quote->current_content, readline("> "));
    while(quote->current_content)
    {
        quote->len++;
        if(ft_strchr(quote->current_content, '"') && counter_quote(quote->current_content, "\"") % 2 == 1)
        {
            expanded = dq_expand_and_concat(quote->current_content, 0, ft_strlen(quote->current_content),shell);

            quote->contents = copy_multiple_input(quote->contents, expanded, quote->len);
            free(expanded);
            break;
        }
        expanded = dq_expand_and_concat(quote->current_content, 0, ft_strlen(quote->current_content),shell);
        quote->contents = copy_multiple_input(quote->contents, expanded, quote->len);
        free(expanded);
        quote->contents = copy_multiple_input(quote->contents, "\n", ++quote->len);
        quote->current_content = set_and_free(quote->current_content, readline("> "));
    }
}

char *double_quote(char **prompt,t_shell *shell)
{
	char	*start;
	char	*end;
	char	*result;
	char	*expanded_first;
	t_quote *quote;

	quote = quote_init();
	start = *prompt;
	end = ft_strchr(start + 1, '"');
	if (!end)
	{
		expanded_first = dq_expand_and_concat(start + 1, 0, ft_strlen(start + 1),shell);
		quote->contents = copy_multiple_input(quote->contents, expanded_first, ++quote->len);
		free(expanded_first);
		quote->contents = copy_multiple_input(quote->contents, "\n", ++quote->len);
		double_quote_loop(quote,shell);
	}
	if(quote->len > 0)
	{
		result = string_concatation_heap(quote->contents);
		result[ft_strlen(result) - 1] = '\0';
		*prompt = start + ft_strlen(start);
		free_quote(quote);
		return result;
	}
	else{
		result = dq_expand_and_concat(start, 1, end - start,shell);
		*prompt = end + 1;
		free_quote(quote);
		return result;
	}
}
char *double_quote_control(char **prompt,t_shell *shell)
{
	char *result;
	char *temp;

	result = double_quote(prompt,shell);
	// Bash mantığı: quote bittikten sonra space yoksa devam eden karakterleri birleştir
	while (**prompt && **prompt != ' ' && **prompt != '\t' && **prompt != '\n' &&
		   **prompt != '>' && **prompt != '<' && **prompt != '|') {
		if (**prompt == '"') {
			temp = double_quote(prompt,shell);
			char *new_result = ft_strjoin(result ? result : "", temp);
			free(result);
			free(temp);
			result = new_result;
		} else if (**prompt == '\'') {
			temp = single_quote_control(prompt,shell);
			char *new_result = ft_strjoin(result ? result : "", temp);
			free(result);
			free(temp);
			result = new_result;
		} else {
			temp = get_characters(prompt,shell);
			char *new_result = ft_strjoin(result ? result : "", temp);
			free(result);
			free(temp);
			result = new_result;
		}
	}
	if (result == NULL)
		return ft_strdup("");
	return result;
}
