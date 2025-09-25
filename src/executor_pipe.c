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

	check_for_errors(pipe((*stack)->pipe), data, *stack, "pipe");
	printf("pipe created pipe[0]=%d pipe[1]=%d\n", (*stack)->pipe[0], (*stack)->pipe[1]);
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
	
	right_in = (*stack)->pipe[0];
	if (get_next_pipe(stack))
		right_out = (*get_next_pipe(stack))->pipe[1];
	else
		right_out = STDOUT_FILENO;
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
	pid_t	res;

	status = 0;
	if (!get_next_pipe(stack))
	{
		if ((*stack)->child_count == 1)
			res = waitpid((*stack)->child_pid[0], &status, 0);
		else
			res = waitpid((*stack)->child_pid[1], &status, 0);
		if (WIFEXITED(status))
			(*stack)->exit_status = WEXITSTATUS(status);
	}
	//for now it stays like this but i still need to check signals
	(*stack)->phase = DONE;
	return (0);
}

int	execute_pipe_done(t_data **data, t_stack **stack)
{
	close((*stack)->pipe[0]);
	close((*stack)->pipe[1]);
	if (!get_next_pipe(stack))
	{
		(*data)->exit_status = (*stack)->exit_status;
		while (errno != ECHILD)
			wait(NULL);
		if ((*stack)->next)
			setup_next_to_top(data, stack);
	}
	pop(stack);
	return (1);
}
