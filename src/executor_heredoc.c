/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 12:49:01 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/28 16:58:11 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_for_heredoc(t_data *data)
{
	push_stack(&data->stack, data->parser_tree, 0, 0, data);
	if (check_heredoc_left(data))
		return (-1);
	if (check_heredoc_right(data))
		return (-1);
	free_stack(&data->stack);
	data->stack = NULL;
	return (0);
}

int	check_heredoc_left(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (0);
	if (push_left_until_cmd(data))
		return (-1);
	if (execute_heredoc(data, data->stack->node->redir))
		return (-1);
	pop(&data->stack);
	return (0);
}

int	check_heredoc_right(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (0);
	while (data->stack)
	{
		if (data->stack->phase == DONE)
		{
			if (execute_heredoc(data, data->stack->node->redir))
				return (-1);
			pop(&data->stack);
		}
		else if (data->stack->node->right)
		{
			data->stack->phase = DONE;
			if (data->stack->node->right)
				push_stack(&data->stack, data->stack->node->right, 0, 0, data);
			if (check_heredoc_left(data))
				return (-1);
		}
	}
	return (0);
}

int	execute_heredoc(t_data *data, t_redir *redir)
{
	pid_t	pid;

	if (!redir)
		return (0);
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (validate_pipe(pipe(data->stack->pipe), &data->stack))
				return (-1);
			pid = fork();
			if (pid < 0)
				return (validate_fork(data, &data->stack));
			else if (pid == 0)
				run_heredoc_child(data, redir);
			else if (run_heredoc_parent(data, redir, pid))
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

t_redir	*get_last_heredoc(t_redir *redir)
{
	t_redir	*res;

	res = NULL;
	while (redir)
	{
		if (redir->type == HEREDOC)
			res = redir;
		redir = redir->next;
	}
	return (res);
}
