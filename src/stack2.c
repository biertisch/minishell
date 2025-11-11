/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:07:45 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 15:07:46 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_stack	**get_first_pipe(t_stack **stack)
{
	t_stack	**head;

	head = stack;
	while (head && (*head))
	{
		if ((*head)->type == NODE_PIPE)
			return (head);
		head = &((*head)->next);
	}
	return (NULL);
}

t_stack	**get_next_pipe(t_stack **stack)
{
	if (!stack || !*stack)
		return (NULL);
	return (get_first_pipe(&(*stack)->next));
}

int	stack_size(t_stack *stack)
{
	int	size;

	size = 0;
	while (stack)
	{
		stack = stack->next;
		size++;
	}
	return (size);
}

void	close_all_open_redir_ends(t_data *data)
{
	t_list	*node;

	node = data->open_redirs;
	while (node)
	{
		if (*(int *)node->content != -1)
			close(*(int *)node->content);
		node = node->next;
	}
}

void	close_all_pipe_ends(t_stack **stack)
{
	t_stack	**head;

	head = stack;
	while (head && (*head))
	{
		if ((*head)->type == NODE_PIPE)
		{
			close((*head)->pipe[0]);
			close((*head)->pipe[1]);
		}
		head = &((*head)->next);
	}
}
