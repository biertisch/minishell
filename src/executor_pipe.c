/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:50:31 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/01 18:08:47 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_pipe(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
		return (execute_pipe_entered(data, stack));
	if ((*stack)->phase == LAUNCH_LEFT)
		return (execute_pipe_launch_left(data, stack));
	if ((*stack)->phase == LAUNCH_RIGHT)
		return (execute_pipe_launch_right(data, stack));
	if ((*stack)->phase == WAIT)
		return (execute_pipe_wait(stack));
	if ((*stack)->phase == DONE)
		return (execute_pipe_done(&data, stack));
	return (0);
}

int	execute_pipe_entered(t_data *data, t_stack **stack)
{
	int	left_in;
	int	left_out;
	
	if (validate_pipe(pipe((*stack)->pipe), stack) == 1)
		return (0);
	left_in = (*stack)->in_fd;
	left_out = ((*stack)->pipe)[1];
	(*stack)->phase = LAUNCH_LEFT;
	push_stack(stack, (*stack)->node->left, left_in, left_out, data);
	return (0);
}

int	execute_pipe_launch_left(t_data *data, t_stack **stack)
{
	int	right_in;
	int	right_out;
	
	if ((*stack)->node->right->type == NODE_SUBSHELL
		&& (*stack)->node->right->left->type == NODE_PIPE)
		right_out = -1;
	else
	{
		if (get_next_pipe(stack) && (*get_next_pipe(stack))->phase != LAUNCH_RIGHT)
			right_out = (*get_next_pipe(stack))->pipe[1];
		else if ((*stack)->out_fd == -1)
		{
			if (get_next_pipe(stack) && (get_next_pipe(get_next_pipe(stack))))
				right_out = (*get_next_pipe(get_next_pipe(stack)))->pipe[1];
			else
				right_out = STDOUT_FILENO;
		}
		else
			right_out = (*stack)->out_fd;
	}
	right_in = (*stack)->pipe[0];
	(*stack)->phase = LAUNCH_RIGHT;
	push_stack(stack, (*stack)->node->right, right_in, right_out, data);
	return (0);
}

int	execute_pipe_launch_right(t_data *data, t_stack **stack)
{
	(void)data;
	close((*stack)->pipe[0]);
	close((*stack)->pipe[1]);
	(*stack)->phase = WAIT;
	return (0);
}

int	execute_pipe_wait(t_stack **stack)
{
	int		status;

	status = 0;
	if (!get_next_pipe(stack))
	{
		if ((*stack)->child_count == 1)
			waitpid((*stack)->child_pid[0], &status, 0);
		else
			waitpid((*stack)->child_pid[1], &status, 0);
		if (WIFEXITED(status))
			(*stack)->exit_status = WEXITSTATUS(status);
	}
	(*stack)->phase = DONE;
	return (0);
}

int	execute_pipe_done(t_data **data, t_stack **stack)
{
	if (!get_next_pipe(stack))
	{
		(*data)->exit_status = (*stack)->exit_status;
		while (1)
		{
			wait(NULL);
			if (errno == ECHILD)
				break;
		}
	}
	if ((*stack)->next)
		setup_next_to_top(data, stack);
	pop(stack);
	return (1);
}
