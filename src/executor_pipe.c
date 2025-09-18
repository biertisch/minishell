/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:50:31 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/18 10:50:37 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_pipe(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
		return (execute_pipe_entered(data, stack));
	if ((*stack)->phase == LAUNCH_LEFT)
		return (execute_pipe_launch_right(data, stack));
	if ((*stack)->phase == LAUNCH_RIGHT)
		return ((*stack)->phase = WAIT, 0);
	if ((*stack)->phase == WAIT)
		return (execute_pipe_wait(stack));
	if ((*stack)->phase == DONE)
		return (execute_pipe_done(stack));
	return (0);
}

int	execute_pipe_entered(t_data *data, t_stack **stack)
{
	int	left_in;
	int	left_out;

	check_for_errors(pipe((*stack)->pipe), data, *stack, "pipe");
	left_in = (*stack)->in_fd;
	left_out = ((*stack)->pipe)[1];
	(*stack)->phase = LAUNCH_LEFT;
	push_stack(stack, (*stack)->node->left, left_in, left_out, data);
	return (0);
}

int	execute_pipe_launch_right(t_data *data, t_stack **stack)
{
	int	right_in;
	int	right_out;

	right_in = (*stack)->old_fd;
	right_out = (*stack)->pipe[1];
	(*stack)->phase = LAUNCH_RIGHT;
	push_stack(stack, (*stack)->node->right, right_in, right_out, data);
	return (0);
}

int	execute_pipe_wait(t_stack **stack)
{
	(*stack)->phase = DONE;
	(void)stack;
	return (0);
}

int	execute_pipe_done(t_stack **stack)
{
	if (!(*stack)->next)
	{
		close((*stack)->pipe[0]);
		close((*stack)->pipe[1]);
		close((*stack)->old_fd);
		return (1);
	}
	if ((*stack)->pipe[0] != (*stack)->old_fd)
		close((*stack)->pipe[0]);
	if ((*stack)->pipe[1] != (*stack)->old_fd)
		close((*stack)->pipe[1]);
	if ((*stack)->next->type == NODE_PIPE)
		(*stack)->next->old_fd = (*stack)->old_fd;
	else
		close((*stack)->old_fd);
	pop(stack);
	return (1);
}
