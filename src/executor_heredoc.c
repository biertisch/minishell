/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 12:49:01 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/28 12:50:14 by beatde-a         ###   ########.fr       */
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
	if (push_left_until_cmd(data, NULL))
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
			push_right_once(data);
			if (check_heredoc_left(data))
				return (-1);
		}
	}
	return (0);
}

int	execute_heredoc(t_data *data, t_redir *redir)
{
	if (!redir)
		return (0);
	while (redir)
	{
		if (redir->type == HEREDOC && dummy_heredoc(data, redir))
		{
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
