/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_subshell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:57:43 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/01 18:09:02 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	int		status;
	
	//delete contents of redir_out files
	(*stack)->phase = DONE;
	pid = fork();
	if (pid < 0)
		return (validate_fork(*data, stack));
	else if (pid == 0)
	{
		subshell_redir(data, stack);
		//add_redir_out but change to append, if it redir_in mantain
		(*stack)->child_count = -42;
		push_stack(stack, (*stack)->node->left, (*stack)->in_fd, (*stack)->out_fd, *data);
	}
	else
	{
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
		}
	}
	return (0);
}

int	subshell_redir(t_data **data, t_stack **stack)
{
	t_redir *redir;
	int		fd;
	int		flag;

	(void)data;
	redir = (*stack)->node->redir;
	while (redir)
	{
		flag = -1;
		if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (redir->type == REDIR_OUT)
				fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0644);
			else
				fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			(*stack)->out_fd = fd;
			flag = 0;
		}
		else if (redir->type == REDIR_IN)
		{
			fd = open(redir->file, O_RDONLY);
			(*stack)->in_fd = fd;
			flag = 1;
		}
		else if (redir->type == HEREDOC)
		{
			(*stack)->in_fd = STDIN_FILENO;
			flag = 2;
		}
		redir = redir->next;
		if (redir && flag != 2)
		{
			if (flag == 0 && (redir->type == REDIR_OUT || redir->type == APPEND))
				close(fd);
			if (flag == 1 && (redir->type == REDIR_IN || redir->type == HEREDOC))
				close(fd);
		}
	}
	return (0);
}

int	execute_subshell_done(t_data **data, t_stack **stack)
{
	int	tree_size;
	int	exit_status;

	if ((*stack)->child_count == -42)
	{
		exit_status = (*stack)->exit_status;
		close_all_pipe_ends(stack);
		free_all(*data);
		free_stack(stack);
		exit(exit_status);
	}
	tree_size = count_tree_nodes((*stack)->node);
	if ((*stack)->next)
		setup_next_to_top(data, stack);
	else
		(*data)->exit_status = (*stack)->exit_status;
	pop(stack);
	return (tree_size);
}
