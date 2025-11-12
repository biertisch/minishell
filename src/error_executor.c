/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:47:52 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/08 11:48:12 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	validate_malloc_execute(t_data *data, t_stack **stack,
	void *ptr, void *to_free)
{
	if (ptr)
		return ;
	undo_duplicate_std(0);
	close_all_open_redir_ends(data);
	close_all_pipe_ends(stack);
	system_error(strerror(errno), "malloc");
	if (to_free)
		free(to_free);
	error_exit(data, stack);
}

int	validate_fork(t_data *data, t_stack **stack)
{
	int	i;

	i = 0;
	if (has_node_type_ancestor(*stack, NODE_PIPE))
	{
		while (has_node_type_ancestor(*stack, NODE_PIPE))
		{
			pop(stack);
			i++;
		}
		data->exit_status = 1;
	}
	else
		(*stack)->exit_status = 0;
	system_error(strerror(errno), "fork");
	return (i);
}

int	validate_pipe(int pipe_res, t_stack **stack)
{
	if (pipe_res == 0)
		return (0);
	close_all_pipe_ends(stack);
	(*stack)->phase = DONE;
	(*stack)->exit_status = 1;
	system_error(strerror(errno), "pipe");
	return (1);
}
