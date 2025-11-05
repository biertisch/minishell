/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:31:30 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:11:27 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	traverse_redir_in(t_data *data, t_stack **stack)
{
	while ((*stack)->node->type == NODE_AND || (*stack)->node->type == NODE_OR)
		push_stack(stack, (*stack)->node->left, 0, 0, data);
	if (check_redir_in_left(data, stack))
		return (-1);
	if (check_redir_in_right(data, stack))
		return (-1);
	while (*stack)
		pop(stack);
	*stack = NULL;
	return (1);
}

int	check_redir_in_left(t_data *data, t_stack **stack)
{
	if (!(*stack) || !(*stack)->node)
		return (-1);
	if (push_left_until_cmd_redir(data, stack))
		return (-1);
	open_redir_in(data, (*stack)->node->redir);
	pop(stack);
	return (0);

}

int	check_redir_in_right(t_data *data, t_stack **stack)
{
	if (!(*stack) || !(*stack)->node)
		return (-1);
	while (*stack)
	{
		if ((*stack)->phase == DONE)
			pop(stack);
		else if ((*stack)->node->right)
		{
			(*stack)->phase = DONE;
			if ((*stack)->node->type == NODE_PIPE)
			{
				push_stack(stack, (*stack)->node->right, 0, 0, data);
				if (check_redir_in_left(data, stack))
					return (-1);
			}
		}
	}
	return (0);
}



int	push_left_until_cmd_redir(t_data *data, t_stack **stack)
{
	while ((*stack)->node->left)
	{
		if ((*stack)->node->type == NODE_SUBSHELL)
		{
			(*stack)->phase = DONE;
			if (open_redir_in(data, (*stack)->node->redir) == 2)
				return (-1);
		}
		push_stack(stack, (*stack)->node->left, 0, 0, data);
	}
	return (0);
}

int	open_redir_in(t_data *data, t_redir *redir)
{
	int	*fd;
	t_list	*new;

	if (!redir)
		return (0);
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			
			if (expand_single_redir(data, redir))
			{
				redir->in_fd = -1;
				return (2);	
			}
			fd = malloc(sizeof(int));
			validate_malloc(data, fd, NULL);
			redir->in_fd = open(redir->file, O_RDONLY);
			*fd = redir->in_fd;
			new = ft_lstnew(fd);
			validate_malloc(data, new, fd);
			if (!data->open_redir_ins)
				data->open_redir_ins = new;
			else
				ft_lstadd_back(&data->open_redir_ins, new);
			if (redir->in_fd == -1)
			{
				handle_open_errors(redir);
				return (2);
			}
		}
		redir = redir->next;
	}
	return (1);
}

int	close_redir_in(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (redir->in_fd == -1)
				break ;
			close(redir->in_fd);
		}
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
	else if (errno == EISDIR)
		write(STDERR_FILENO, ": Is a directory\n", 17);
}
