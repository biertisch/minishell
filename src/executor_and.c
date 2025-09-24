/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_and.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 13:43:14 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/20 13:43:15 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_and(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
		return (execute_and_entered(data, stack));
	if ((*stack)->phase == LAUNCH_LEFT)
		return (execute_and_launch_left(data, stack));
	if ((*stack)->phase == LAUNCH_RIGHT)
		return (execute_and_launch_right(data, stack));
	if ((*stack)->phase == DONE)
		return (execute_and_done(&data, stack));
	return (1);
}

int	execute_and_entered(t_data *data, t_stack **stack)
{
	(*stack)->phase = LAUNCH_LEFT;
	push_stack(stack, (*stack)->node->left, STDIN_FILENO, STDOUT_FILENO, data);
	return (0);
}

int	execute_and_launch_left(t_data *data, t_stack **stack)
{
	(*stack)->phase = LAUNCH_RIGHT;
	if (!(*stack)->exit_status)
		push_stack(stack, (*stack)->node->right, STDIN_FILENO, STDOUT_FILENO, data);
	return (0);
}

int	execute_and_launch_right(t_data *data, t_stack **stack)
{
	(void)data;
	(*stack)->phase = DONE;
	return (0);
}

int	execute_and_done(t_data **data, t_stack **stack)
{
	int	right_tree_size;

	if ((*stack)->next)
		setup_next_to_top(data, stack);
	else
		(*data)->exit_status = (*stack)->exit_status;
	if (!(*data)->exit_status)
	{
		pop(stack);
		return (1);
	}
	else
	{
		right_tree_size = count_tree_nodes((*stack)->node->right);
		pop(stack);
		return (1 + right_tree_size);
	}
}
