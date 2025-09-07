/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:15:44 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/02 15:15:45 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	return (head);
}

void	push_stack(t_stack **stack, t_tree *node, int in_fd, int out_fd, t_data *data)
{
	t_stack *new_head;

	new_head = malloc(sizeof(t_stack));
	validate_malloc(data, new_head, NULL);
	new_head->phase = ENTERED;
	new_head->type = node->type;
	new_head->node = node;
	new_head->in_fd = in_fd;
	new_head->out_fd = out_fd;
//	if (new_head->type == NODE_PIPE && !has_pipe_ancestor(stack)) //is this check required?
		new_head->child_count = 0;
	new_head->next = *stack;
	*stack = new_head;
}

int	has_pipe_ancestor(t_stack *stack)
{
	t_stack	*curr;

	curr = stack;
	while (curr)
	{
		if (curr->type == NODE_PIPE)
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	pop(t_stack **stack)
{
	t_stack *new_head;

	if (stack == NULL || *stack == NULL)
		return ;
	new_head = (*stack)->next;
	free((*stack));
	*stack = new_head;
}

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

//use ONLY for debug
/**
void	print_stack(t_stack *stack)
{
	while (stack)
	{
		ft_printf("hey!\n\n\n\n");
		print_parser_node(stack->node, 0, "cona");
		stack = stack->next;
	}
}*/
