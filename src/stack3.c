/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:07:52 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 15:07:53 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pop(t_stack **stack)
{
	t_stack	*new_head;

	if (stack == NULL || *stack == NULL)
		return ;
	new_head = (*stack)->next;
	free((*stack));
	*stack = new_head;
}

t_fd_pair	get_fd_pair(int in_fd, int out_fd)
{
	return ((t_fd_pair){in_fd, out_fd});
}

t_stack	**get_first_subshell(t_stack **stack)
{
	t_stack	**head;

	head = stack;
	while (head && (*head))
	{
		if ((*head)->type == NODE_SUBSHELL)
			return (head);
		head = &((*head)->next);
	}
	return (NULL);
}

int	is_last_cmd_in_pipe(t_stack **stack)
{
	t_stack	**next_pipe;
	t_stack	**next_next_pipe;

	if (!stack || !*stack)
		return (0);
	if ((*stack)->type != NODE_CMD)
		return (0);
	next_pipe = get_next_pipe(stack);
	if (!next_pipe || !*next_pipe)
		return (1);
	next_next_pipe = get_next_pipe(next_pipe);
	if (!next_next_pipe && (*next_pipe)->phase == LAUNCH_RIGHT)
		return (1);
	return (0);
}

t_stack	**get_next_pipe_in_subshell(t_stack **stack)
{
	t_stack	**head;

	if (!stack || !*stack)
		return (NULL);
	head = &((*stack)->next);
	while (head && (*head))
	{
		if ((*head)->type == NODE_SUBSHELL)
			return (NULL);
		if ((*head)->type == NODE_PIPE)
			return (head);
		head = &((*head)->next);
	}
	return (NULL);
}
