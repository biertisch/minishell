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

int	setup_next_to_top(t_data **data, t_stack **stack)
{
	if ((*stack)->next->type == NODE_SUBSHELL)
			exit((*stack)->exit_status);
	if ((*stack)->next->type == NODE_AND)
	{
		if ((*stack)->next->phase == LAUNCH_LEFT)
			(*stack)->next->exit_status = (*stack)->exit_status;
		if ((*stack)->next->phase == LAUNCH_RIGHT)
			(*stack)->next->exit_status = (*stack)->next->exit_status || (*stack)->exit_status;
	}
	else if ((*stack)->next->type == NODE_OR)
	{
		if ((*stack)->next->phase == LAUNCH_LEFT)
			(*stack)->next->exit_status = (*stack)->exit_status;
		if ((*stack)->next->phase == LAUNCH_RIGHT)
			(*stack)->next->exit_status = (*data)->exit_status || (*stack)->exit_status;	
	}
	return (0);
}

t_stack	**get_first_log_operator(t_stack **stack)
{
	t_stack **head;

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

void	pop(t_stack **stack)
{
	t_stack *new_head;

	if (stack == NULL || *stack == NULL)
		return ;
	new_head = (*stack)->next;
	free((*stack));
	*stack = new_head;
}

t_stack	*peek(t_stack **stack)
{
	if (stack == NULL || *stack == NULL)
		return (NULL);
	return (*stack);
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

t_stack **get_next_pipe(t_stack **stack)
{
  	if (!stack || !*stack)
		return NULL;
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

void	close_all_pipe_ends(t_stack **stack)
{
	t_stack **head;

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

static char	*type_to_string(t_node_type type)
{
	if (type == NODE_CMD)
		return "NODE_CMD";
	if (type == NODE_BUILTIN)
		return "NODE_BUILTIN";
	if (type == NODE_PIPE)
		return "NODE_PIPE";
	else
		return "ERROR_TYPE";
}

static char	*phase_to_string(t_phase phase)
{
	if (phase == ENTERED)
		return "ENTERED";
	if (phase == LAUNCH_LEFT)
		return "LAUNCH_LEFT";
	if (phase == LAUNCH_RIGHT)
		return "LAUNCH_RIGHT";
	if (phase == WAIT)
		return "WAIT";
	if (phase == DONE)
		return "DONE";
	else
		return "ERROR_PHASE";
}

void	print_top(t_stack *stack)
{

	ft_printf("STACK TOP: [TYPE=%s PHASE=%s PIPE[0]=%d PIPE[1]=%d]\n", type_to_string(stack->type), phase_to_string(stack->phase), stack->pipe[0], stack->pipe[1]);
}

void	print_stack(t_stack *stack)
{
	int	i;

	i = 0;
	while (stack)
	{
		printf("\nlevel %d: TYPE=%s PHASE=%s", i, type_to_string(stack->type), phase_to_string(stack->phase));
		if (stack->type == NODE_PIPE)
			printf(" PIPE[0]=%d PIPE[1]=%d\n", stack->pipe[0], stack->pipe[1]);
		else
			printf("\n");
		i++;
		stack = stack->next;
	}
	ft_printf("STACK DONE!\n");
}
