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
	
	if ((*stack)->next && (*stack)->next->phase != LAUNCH_LEFT && ((*stack)->next->type != NODE_AND && (*stack)->next->type != NODE_OR))
	{
		if ((*stack)->in_fd != STDIN_FILENO)
			close((*stack)->in_fd);
		if ((*stack)->out_fd != STDOUT_FILENO)
			close((*stack)->out_fd);
	}
	close_all_pipe_ends(stack);
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*stack)->exit_status = WEXITSTATUS(status);
	if ((*stack)->next && (*stack)->next->type == NODE_SUBSHELL)
		(*stack)->next->exit_status = (*stack)->exit_status;
	return (1);
}

int	parent_heredoc(t_stack **stack, pid_t pid)
{
	char	*heredoc_input;

	heredoc_input = get_last_heredoc((*stack)->node->redir)->heredoc_input;
	close((*stack)->pipe[0]);
	write((*stack)->pipe[1], heredoc_input, ft_strlen(heredoc_input));
	close((*stack)->pipe[1]);
	if (get_next_pipe(stack))
		return (parent(stack, pid));
	return (parent_single_command(stack, pid));
}
