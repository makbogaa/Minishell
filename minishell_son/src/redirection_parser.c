/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:45:00 by makboga           #+#    #+#             */
/*   Updated: 2025/07/30 16:17:12 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Parse edilmiş command'ları redirection'lar için process et
void process_redirections(t_shell *shell)
{
    t_command *cmd;
    t_command *next_cmd;
    t_redirect *redirect;
    
    cmd = shell->command_p;
    
    while (cmd)
    {
        // Eğer bu command'ın bir redirection token'ı varsa
        if (cmd->token_flag && cmd->token && is_redirect_token(cmd->token))
        {
            next_cmd = cmd->next;
            
            // Sonraki command filename olmalı
            if (next_cmd && next_cmd->command)
            {
                // Redirection oluştur ve mevcut command'a ekle
                redirect = create_redirect(get_redirect_type(cmd->token), next_cmd->command);
                if (redirect)
                {
                    add_redirect(cmd, redirect);
                    
                    // Token'ı temizle
                    free(cmd->token);
                    cmd->token = NULL;
                    cmd->token_flag = 0;
                    
                    // Filename command'ını listeden çıkar
                    cmd->next = next_cmd->next;
                    
                    if (next_cmd->command)
                        free(next_cmd->command);
                    free(next_cmd);
                    
                    // Aynı command'dan devam et
                    continue;
                }
            }
        }
        
        // Ayrıca parameters içinde de redirection kontrol et
        if (cmd->parameters_p)
        {
            t_parameters *param = cmd->parameters_p;
            t_parameters *prev_param = NULL;
            
            while (param && param->next)
            {
                if (is_redirect_token(param->parameter))
                {
                    redirect = create_redirect(get_redirect_type(param->parameter), param->next->parameter);
                    if (redirect)
                    {
                        add_redirect(cmd, redirect);
                        
                        // Remove redirection tokens from parameters
                        t_parameters *next_param = param->next->next;
                        
                        free(param->parameter);
                        free(param->next->parameter);
                        free(param->next);
                        
                        if (prev_param)
                            prev_param->next = next_param;
                        else
                            cmd->parameters_p = next_param;
                        
                        free(param);
                        param = next_param;
                        continue;
                    }
                }
                prev_param = param;
                param = param->next;
            }
        }
        
        cmd = cmd->next;
    }
}

// Parametre listesinden bir parametreyi çıkar
void remove_parameter(t_parameters **head, t_parameters *to_remove)
{
    t_parameters *current;
    t_parameters *prev;
    
    if (!head || !*head || !to_remove)
        return;
        
    // İlk eleman mı?
    if (*head == to_remove)
    {
        *head = (*head)->next;
        free(to_remove->parameter);
        free(to_remove);
        return;
    }
    
    // Listede ara
    prev = *head;
    current = (*head)->next;
    while (current)
    {
        if (current == to_remove)
        {
            prev->next = current->next;
            free(current->parameter);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
