/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_parent_redir.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 15:38:02 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/02 15:38:04 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_parent_redir(t_data *data, t_stack **stack)
{
	t_redir	*redir;
	int		saved_fd;
	int		res;

	res = 1;
	saved_fd = -1;
	while (redir && res)
	{
		if (redir->type == REDIR_IN)
			res = execute_parent_redir_in(data, stack, redir, &saved_fd);
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
			res = execute_parent_redir_out(data, stack, redir);
	}
}

int	execute_parent_redir_in(t_data *data, t_stack **stack,
			t_redir *redir, int *saved_fd)
{
	int	s;

	if (redir->in_fd == -1)
		return (0);
	s = dup(STDIN_FILENO);
	dup2(redir->in_fd, STDIN_FILENO);
	*saved_fd = s;
	return (s);
}
