/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_preprocessor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:00:00 by makboga           #+#    #+#             */
/*   Updated: 2025/08/14 18:00:36 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Redirection'ları komutun sonuna taşı
// ">file echo hi" -> "echo hi >file"  
char *normalize_redirections(char *command)
{
    if (!command)
        return (NULL);
        
    // Basit kontrol: komut redirection ile başlıyorsa
    while (*command == ' ' || *command == '\t')
        command++;
        
    if (*command == '>' || *command == '<')
    {
        // Redirection'ları komutun sonuna taşımak yerine
        // Bu durumda command'ı olduğu gibi döndür 
        // Parse logic'i bunu handle etmeli
        return (ft_strdup(command));
    }
    
    return (ft_strdup(command));
}
