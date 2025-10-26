/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:31:30 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/24 17:31:32 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	traverse_redir_in(t_data *data, t_stack **stack)
{
	int	nodes;

	nodes = 1;
	while ((*stack)->node->type == NODE_AND || (*stack)->node->type == NODE_OR)
	{
		push_stack(stack, (*stack)->node->left, 0, 0, data);
		nodes++;
	}
	while (nodes && (*stack)->node->type == NODE_PIPE)
	{
		if ((*stack)->phase != LAUNCH_LEFT)
		{
			(*stack)->phase = LAUNCH_RIGHT;
			push_stack(stack, (*stack)->node->right, 0, 0, data);
			nodes++;
		}
		open_redir_in(stack, (*stack)->node->redir, &nodes);
		if (nodes)
		{
			if ((*stack)->node->type == NODE_PIPE)
				(*stack)->phase = LAUNCH_LEFT;
			push_stack(stack, (*stack)->node->left, 0, 0, data);
			nodes++;
			open_redir_in(stack, (*stack)->node->redir, &nodes);
		}

	}
	while (nodes)
		open_redir_in(stack, (*stack)->node->redir, &nodes);
	return (1);
}

int	open_redir_in(t_stack **stack, t_redir *redir, int *nodes)
{
	if ((*stack)->node->type == NODE_PIPE && (*stack)->phase == ENTERED)
		return (0);
	else if ((*stack)->node->type == NODE_PIPE)
	{
		pop(stack);
		(*nodes)--;
		return (2);
	}
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			redir->in_fd = open(redir->file, O_RDONLY);
			if (redir->in_fd == -1)
			{
				handle_open_errors(redir);
				break ;
			}
		}
		redir = redir->next;		
	}
	pop(stack);
	(*nodes)--;
	return (1);
}

int	close_redir_in(t_redir *redir)
{
	while (redir)
	{
		if (redir->in_fd == -1)
			break ;
		if (redir->type == REDIR_IN)
			close(redir->in_fd);
		redir = redir->next;
	}
	return (1);
}

void	handle_open_errors(t_redir *redir)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, redir->file, ft_strlen(redir->file));
	if (errno == ENOENT)
		write(STDERR_FILENO, ": No such file or directory\n", 28);
	else if (errno == EACCES)
		write(STDERR_FILENO, ": Permission denied\n", 20);
}
