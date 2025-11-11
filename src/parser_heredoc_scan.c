/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_scan.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 12:49:01 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/10 12:43:16 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	scan_heredocs(t_data *data, t_tree *parser_tree)
{
	if (!parser_tree)
		return (0);
	push_stack(&data->stack, parser_tree, 0, 0, data);
	if (scan_heredocs_left(data))
		return (-1);
	if (scan_heredocs_right(data))
		return (-1);
	free_stack(&data->stack);
	data->stack = NULL;
	return (0);
}

int	scan_heredocs_left(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (0);
	if (push_left_until_cmd(data))
		return (-1);
	if (setup_heredoc(data, data->stack->node->redir))
		return (-1);
	pop(&data->stack);
	return (0);
}

int	scan_heredocs_right(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (0);
	while (data->stack)
	{
		if (data->stack->phase == DONE || !data->stack->node->right)
		{
			if (setup_heredoc(data, data->stack->node->redir))
				return (-1);
			pop(&data->stack);
		}
		else
		{
			data->stack->phase = DONE;
			if (data->stack->node->right)
				push_stack(&data->stack, data->stack->node->right, 0, 0, data);
			if (scan_heredocs_left(data))
				return (-1);
		}
	}
	return (0);
}

int	setup_heredoc(t_data *data, t_redir *redir)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!redir)
		return (0);
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			if (pipe(pipe_fd))
				return (system_error(strerror(errno), "pipe"));
			pid = fork();
			if (pid < 0)
				return (system_error(strerror(errno), "fork"));
			else if (pid == 0)
				run_heredoc_child(data, redir, pipe_fd);
			else if (run_heredoc_parent(data, redir, pipe_fd, pid))
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
