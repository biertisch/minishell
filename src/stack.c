/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:15:44 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/07 13:07:01 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_stack	*create_stack(t_data *data)
{
	t_stack	*head;

	head = malloc(sizeof(t_stack));
	validate_malloc(data, head, NULL);
	head->phase = ENTERED;
	head->type = (data->parser_tree)->type;
	head->node = data->parser_tree;
	head->child_count = 0;
	head->child_pid[0] = -1;
	head->child_pid[1] = -1;
	head->in_fd = STDIN_FILENO;
	head->out_fd = STDOUT_FILENO;
	head->next = NULL;
	head->exit_status = 0;
	return (head);
}

void	push_stack(t_stack **stack, t_tree *node, t_fd_pair fds, t_data *data)
{
	t_stack	*new_head;

	new_head = malloc(sizeof(t_stack));
	validate_malloc_execute(data, stack, new_head, NULL);
	new_head->phase = ENTERED;
	new_head->type = node->type;
	new_head->node = node;
	new_head->in_fd = fds.in_fd;
	new_head->out_fd = fds.out_fd;
	new_head->child_count = 0;
	new_head->next = *stack;
	new_head->exit_status = 0;
	*stack = new_head;
}

int	setup_next_to_top(t_data **data, t_stack **stack)
{
	if ((*stack)->type == NODE_SUBSHELL)
		(*data)->exit_status = (*stack)->exit_status;
	if ((*stack)->next->type == NODE_AND)
		(*stack)->next->exit_status = (*stack)->exit_status;
	else if ((*stack)->next->type == NODE_OR)
		(*stack)->next->exit_status = (*stack)->exit_status;
	else if (!((*stack)->type == NODE_SUBSHELL
			&& ((*stack)->next->type == NODE_PIPE)))
		(*stack)->next->exit_status = (*stack)->exit_status;
	return (0);
}

t_stack	**get_first_log_operator(t_stack **stack)
{
	t_stack	**head;

	head = stack;
	while (head && (*head))
	{
		if ((*head)->type == NODE_OR || (*head)->type == NODE_AND)
			return (head);
		head = &((*head)->next);
	}
	return (NULL);
}

int	has_node_type_ancestor(t_stack *stack, t_node_type type)
{
	t_stack	*curr;

	curr = stack;
	while (curr)
	{
		if (curr->type == type)
			return (1);
		curr = curr->next;
	}
	return (0);
}
