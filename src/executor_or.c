/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_or.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:07:50 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/23 13:07:52 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_or(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
		return (execute_or_entered(data, stack));
	if ((*stack)->phase == LAUNCH_LEFT)
		return (execute_or_launch_left(data, stack));
	if ((*stack)->phase == LAUNCH_RIGHT)
		return (execute_or_launch_right(data, stack));
	if ((*stack)->phase == DONE)
		return (execute_or_done(data, stack));
	return (1);
}

int	execute_or_entered(t_data *data, t_stack **stack)
{
	(*stack)->phase = LAUNCH_LEFT;
	push_stack(stack, (*stack)->node->left, STDIN_FILENO, STDOUT_FILENO, data);
	return (0);
}

int	execute_or_launch_left(t_data *data, t_stack **stack)
{
	(*stack)->phase = LAUNCH_RIGHT;
	(*stack)->exit_status = data->exit_status;
	if ((*stack)->exit_status)
		push_stack(stack, (*stack)->node->right, STDIN_FILENO, STDOUT_FILENO, data);
	return (0);
}

int	execute_or_launch_right(t_data *data, t_stack **stack)
{
	if ((!(*stack)->exit_status || !data->exit_status))
		data->exit_status = 0;
	(*stack)->phase = DONE;
	return (0);
}

int	execute_or_done(t_data *data, t_stack **stack)
{
	int	right_tree_size;

	if (!(*stack)->exit_status)
	{
		right_tree_size = count_tree_nodes((*stack)->node->right);
		pop(stack);
		return (1 + right_tree_size);
	}
	else
		pop(stack);
	(void)data;
	return (1);
}
