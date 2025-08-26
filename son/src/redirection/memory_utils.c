/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makboga <makboga@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:41:38 by makboga           #+#    #+#             */
/*   Updated: 2025/08/25 14:41:38 by makboga          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	clean_exit(int pipe_fd, char *delim_copy, t_req *req, int exit_code)
{
	close(pipe_fd);
	free(delim_copy);
	free_req(req);
	exit(exit_code);
}

void	free_req(t_req *req)
{
	if (!req)
		return ;
	req->shell = NULL;
}
