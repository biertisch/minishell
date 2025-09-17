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
	execute_stack(data, &stack);
	return (1);
}

int	execute_stack(t_data *data, t_stack **stack)
{
	int	i;
	int	tree_nodes_count;

	tree_nodes_count = count_tree_nodes(data->parser_tree);
	i = 0;
	print_parser_tree(data->parser_tree);
	while (i < tree_nodes_count)
	{
		if ((*stack)->type == NODE_PIPE)
			i += execute_pipe(data, stack);
		else if ((*stack)->type == NODE_CMD)
			i += execute_cmd(data, stack);
	}
	return (0);
}

int	execute_cmd(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
		return (execute_cmd_entered(data, stack));
	if ((*stack)->phase == DONE)
		return (execute_cmd_done(stack));
	return (0);
}

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
	/*
	int	status;

	if ((*stack)->child_pid[0] != -1)
		waitpid((*stack)->child_pid[0], &status, 0); 
	if ((*stack)->child_pid[1] != -1)
		waitpid((*stack)->child_pid[1], &status, 0);

	*/
	(*stack)->phase = DONE;
	(void)stack;
	return (0);
}

int	execute_pipe_done(t_stack **stack)
{
	if ((*stack)->pipe[0] != (*stack)->old_fd)
		close((*stack)->pipe[0]);
	if ((*stack)->pipe[1] != (*stack)->old_fd)
		close((*stack)->pipe[1]);
	if ((*stack)->next && (*stack)->next->type == NODE_PIPE)
		(*stack)->next->old_fd = (*stack)->old_fd;
	pop(stack);
	return (1);
}

int	execute_cmd_entered(t_data *data, t_stack **stack)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		check_for_errors(-1, data, *stack, "fork");
	else if (pid == 0)
		child(data, stack);
	else
		parent(stack, pid);
	(*stack)->phase = DONE;
	return (0);
}

int	execute_cmd_done(t_stack **stack)
{
	pop(stack);
	return (1);
}
