/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:37:51 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/02 12:37:53 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute(t_data *data)
{
	t_stack	*stack;

	stack = create_stack(data);
	if (stack->type == NODE_PIPE)
		execute_pipe(data, stack);
	return (0);
}

int	execute_pipe(t_data *data, t_stack *stack)
{
	if (stack->phase == ENTERED)
		execute_pipe_entered(data, stack);
	return (0);
}

int	execute_pipe_entered(t_data *data, t_stack *stack)
{
	int	left_in;
	int	left_out;
	int	right_in;
	int	right_out;

	check_for_errors(pipe(stack->pipe), data, stack, "pipe");
	left_in = stack->in_fd;
	left_out = (stack->pipe)[1];
	right_in = (stack->pipe)[0];
	right_out = stack->out_fd;
	stack->phase = LAUNCH_LEFT;
	push_stack(&stack, stack->node->left, left_in, left_out, data);
	consume_stack(data, &stack);
	push_stack(&stack, stack->node->right, right_in, right_out, data);
	consume_stack(data, &stack);
	return (0);
}

void	consume_stack(t_data *data, t_stack **stack)
{
	pid_t	pid;

	if ((*stack)->type == NODE_CMD)
	{
		pid = fork();
		if (pid < 0)
			check_for_errors(-1, data, *stack, "fork");
		else if (pid == 0)
			child(data, stack);
		//else
		//	parent(stack, pid);
		(*stack)->phase = DONE;
	}
	if ((*stack)->phase == DONE)
		pop(stack);
}
