/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_subshell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:57:43 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/13 21:44:28 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_subshell(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
		return (execute_subshell_entered(&data, stack));
	else if ((*stack)->phase == DONE)
		return (execute_subshell_done(&data, stack));
	return (0);
}

int	execute_subshell_entered(t_data **data, t_stack **stack)
{
	pid_t	pid;

	(*stack)->phase = DONE;
	if (check_redir_in_subshell(stack))
	{
		(*stack)->exit_status = 1;
		return (0);
	}
	pid = fork();
	if (pid < 0)
		return (validate_fork(*data, stack));
	else if (pid == 0)
	{
		setup_signals_child(*data);
		subshell_redir(stack, (*stack)->node->redir);
		(*stack)->child_count = -42;
		push_stack(stack, (*stack)->node->left,
			get_fd_pair((*stack)->in_fd, (*stack)->out_fd), *data);
	}
	else
		execute_subshell_entered_parent(stack, pid);
	return (0);
}

void	execute_subshell_entered_parent(t_stack **stack,
			pid_t pid)
{
	int	status;

	close_redir_in((*stack)->node->redir);
	if ((*stack)->next && (*stack)->next->type == NODE_PIPE)
		(*stack)->next->child_pid[(*stack)->next->child_count++] = pid;
	else
	{
		if ((*stack)->next && (*stack)->next->type == NODE_PIPE)
			close_all_pipe_ends(&((*stack)->next));
		status = 0;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(*stack)->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			(*stack)->exit_status = WTERMSIG(status) + 128;
		handle_child_exit(status);
	}
}

int	subshell_redir(t_stack **stack, t_redir *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == APPEND)
			(*stack)->out_fd = redir->out_fd;
		else if (redir->type == REDIR_IN)
		{
			(*stack)->in_fd = redir->in_fd;
			if (redir->in_fd == -1)
				break ;
		}
		else if (redir->type == HEREDOC)
			(*stack)->in_fd = STDIN_FILENO;
		redir = redir->next;
	}
	return (0);
}

int	check_redir_in_subshell(t_stack **stack)
{
	t_redir	*redir;

	redir = (*stack)->node->redir;
	while (redir)
	{
		if (redir->type == REDIR_IN && redir->in_fd == -1)
			return (1);
		redir = redir->next;
	}
	return (0);
}
