/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:49:39 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/01 18:08:41 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	parent(t_stack **stack, pid_t pid)
{
	if ((*stack)->next && (*stack)->next->type == NODE_SUBSHELL)
		return (parent_single_command(stack, pid));
	if (get_first_pipe(stack))
		(*get_first_pipe(stack))->child_pid[(*get_first_pipe(stack))->child_count++] = pid;
	else
		return (parent_single_command(stack, pid));
	return (1);
}

int	parent_single_command(t_stack **stack, pid_t pid)
{
	int		status;

	close_all_pipe_ends(stack);
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*stack)->exit_status = WEXITSTATUS(status);
	if ((*stack)->next && (*stack)->next->type == NODE_SUBSHELL)
		(*stack)->next->exit_status = (*stack)->exit_status;
	return (1);
}

int	heredoc_loop(t_stack **stack, char *heredoc, char *res)
{
	char	*line;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line || !ft_strcmp(line, heredoc))
			break ;
		else
		{
			res = ft_strdup_append(NULL, res, line);
			free(line);
		}
	}
	write((*stack)->pipe[1], "\n", 1);
	write((*stack)->pipe[1], res, ft_strlen(res));
	if (line)
		free(line);
	free(res);
	free(heredoc);
	return (1);
}

int	parent_heredoc(t_stack **stack, pid_t pid)
{
	char	*line;
	char	*res;
	char	*heredoc;

	close((*stack)->pipe[0]);
	line = get_next_line(STDIN_FILENO);
	heredoc = ft_strdup_append(NULL, ft_strdup((*stack)->node->redir->file), "\n");
	if (ft_strcmp(line, heredoc))
	{
		res = ft_strdup(line);
		free(line);
		heredoc_loop(stack, heredoc, res);
	}
	else
		write((*stack)->pipe[1], "\n", 1);
	close((*stack)->pipe[1]);
	return (parent_single_command(stack, pid));
}
