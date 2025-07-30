/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:18:59 by makboga           #+#    #+#             */
/*   Updated: 2025/07/30 18:13:43 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int builtin_env(char **envp)
{
    int i = 0;

    while (envp && envp[i])
    {
        // __EXPORT_ONLY__ marker'lı entry'leri env'de gösterme
        if (!ft_strstr(envp[i], "=__EXPORT_ONLY__"))
        {
            write(1, envp[i], ft_strlen(envp[i]));
            write(1, "\n", 1);
        }
        i++;
    }
    return 0;
}
