/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:45:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/12 19:41:17 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Parse edilmiş command'ları redirection'lar için process et
void process_redirections(t_shell *shell)
{
    t_command *cmd;
    t_command *next_cmd;
    t_redirect *redirect;
    
    // printf("DEBUG: process_redirections() called\n"); // DEBUG
    
    cmd = shell->command_p;
    
    while (cmd)
    {
        // printf("DEBUG: Processing command: %s\n", cmd->command); // DEBUG
        
        // Ayrıca parameters içinde de redirection kontrol et
        if (cmd->parameters_p)
        {
            t_parameters *param = cmd->parameters_p;
            t_parameters *prev_param = NULL;
            
            // printf("DEBUG: Command has parameters:\n"); // DEBUG
            // t_parameters *debug_param = cmd->parameters_p; // DEBUG
            // while (debug_param) { // DEBUG
            //     printf("DEBUG:   param: '%s'\n", debug_param->parameter); // DEBUG
            //     debug_param = debug_param->next; // DEBUG
            // } // DEBUG
            
            while (param)
            {
                if (param->parameter && is_redirect_token(param->parameter) && param->next && param->next->parameter)
                {
                    // printf("DEBUG: Found redirection in parameters: '%s' -> '%s'\n", param->parameter, param->next->parameter); // DEBUG
                    redirect = create_redirect(get_redirect_type(param->parameter), param->next->parameter);
                    if (redirect)
                    {
                        // printf("DEBUG: Created redirect and adding to command\n"); // DEBUG
                        add_redirect(cmd, redirect);
                        
                        // Bu parameter ve sonraki parameter'ı (filename) kaldır
                        t_parameters *to_remove1 = param;
                        t_parameters *to_remove2 = param->next;
                        t_parameters *next_param = param->next->next;
                        
                        if (prev_param)
                            prev_param->next = next_param;
                        else
                            cmd->parameters_p = next_param;
                        
                        free(to_remove1->parameter);
                        free(to_remove1);
                        free(to_remove2->parameter);
                        free(to_remove2);
                        
                        param = next_param;
                        continue;
                    }
                }
                prev_param = param;
                param = param->next;
            }
        }
        
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
                    
                    // Filename command'ı da kontrol et - eğer onun da token'ı varsa
                    if (next_cmd->token_flag && next_cmd->token && is_redirect_token(next_cmd->token))
                    {
                        // Bu durumda sonraki command'ın token'ını mevcut command'a taşı
                        cmd->token = ft_strdup(next_cmd->token);
                        cmd->token_flag = 1;
                        
                        // Filename command'ını sadece command'dan temizle
                        cmd->next = next_cmd->next;
                        free(next_cmd->command);
                        if (next_cmd->token)
                            free(next_cmd->token);
                        free(next_cmd);
                        
                        // Aynı command'dan devam et (token'ı henüz var)
                        continue;
                    }
                    else
                    {
                        // Normal durum: filename command'ını listeden çıkar
                        cmd->next = next_cmd->next;
                        
                        if (next_cmd->command)
                            free(next_cmd->command);
                        free(next_cmd);
                        
                        // Aynı command'dan devam et
                        continue;
                    }
                }
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
