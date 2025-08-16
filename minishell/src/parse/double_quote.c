/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:07:25 by mdalkili          #+#    #+#             */
/*   Updated: 2025/08/16 15:27:45 by makboga          ###   ########.fr       */
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
        result = set_and_free(result, ft_strjoin(result ? result : "", tmp));
        free(tmp);
    }
    return result ? result : ft_strdup("");
}

static void double_quote_loop(t_quote *quote,t_shell *shell)
{
	char *expanded;
	
	quote->current_parameter = set_and_free(quote->current_parameter, readline("> "));
    while(quote->current_parameter)
    {
        quote->len++;
        if(ft_strchr(quote->current_parameter, '"') && counter_quote(quote->current_parameter, "\"") % 2 == 1)
        {
            expanded = dq_expand_and_concat(quote->current_parameter, 0, ft_strlen(quote->current_parameter),shell);

            quote->parameters = copy_multiple_input(quote->parameters, expanded, quote->len);
            free(expanded);
            break;
        }
        expanded = dq_expand_and_concat(quote->current_parameter, 0, ft_strlen(quote->current_parameter),shell);
        quote->parameters = copy_multiple_input(quote->parameters, expanded, quote->len);
        free(expanded);
        quote->parameters = copy_multiple_input(quote->parameters, "\n", ++quote->len);
        quote->current_parameter = set_and_free(quote->current_parameter, readline("> "));
    }
}

static char *double_quote(char **prompt,t_shell *shell)
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
		quote->parameters = copy_multiple_input(quote->parameters, expanded_first, ++quote->len);
		free(expanded_first);
		quote->parameters = copy_multiple_input(quote->parameters, "\n", ++quote->len);
		double_quote_loop(quote,shell);
	}
	if(quote->len > 0)
	{
		result = string_concatation_heap(quote->parameters);
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
			result = set_and_free(result, ft_strjoin(result, temp));
			free(temp);
		} else if (**prompt == '\'') {
			temp = single_quote_control(prompt,shell);
			result = set_and_free(result, ft_strjoin(result, temp));
			free(temp);
		} else {
			temp = get_characters(prompt,shell);
			result = set_and_free(result, ft_strjoin(result, temp));
			free(temp);
		}
	}
	if (result == NULL)
		return ft_strdup("");
	return result;
}
