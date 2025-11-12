/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 12:46:30 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 20:36:35 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_redir_in(t_data *data, t_stack **stack, char *cmd, t_redir *redir)
{
	if (redir->in_fd == -1)
	{
		if ((*stack)->node->argv && !has_node_type_ancestor(*stack, NODE_PIPE)
			&& is_builtin_no_fork((*stack)->node->argv[0]))
			return ;
		executor_cleanup(data, stack, cmd);
		exit(1);
	}
	dup2(redir->in_fd, STDIN_FILENO);
	if ((*stack)->out_fd != STDOUT_FILENO)
		dup2((*stack)->out_fd, STDOUT_FILENO);
	if ((*stack)->out_fd != STDOUT_FILENO)
		close((*stack)->out_fd);
	if (!redir->next)
		child_execute(data, stack, cmd);
}

void	child_redir_out(t_data *data, t_stack **stack,
		char *cmd, t_redir *redir)
{
	if (redir->out_fd == -1)
	{
		if ((*stack)->node->argv && !has_node_type_ancestor(*stack, NODE_PIPE)
			&& is_builtin_no_fork((*stack)->node->argv[0]))
			return ;
		executor_cleanup(data, stack, cmd);
		exit(1);
	}
	if (redir->fd == -1)
		dup2(redir->out_fd, STDOUT_FILENO);
	if (redir->fd == -1)
		close(redir->out_fd);
	if ((*stack)->in_fd != STDIN_FILENO)
		dup2((*stack)->in_fd, STDIN_FILENO);
	if ((*stack)->in_fd != STDIN_FILENO)
		close((*stack)->in_fd);
	if (!redir->next)
		child_execute(data, stack, cmd);
}

void	child_heredoc(t_data *data, t_stack **stack, char *cmd, t_redir *redir)
{
	dup2((*stack)->pipe[0], STDIN_FILENO);
	if ((*stack)->out_fd != STDOUT_FILENO)
		dup2((*stack)->out_fd, STDOUT_FILENO);
	if ((*stack)->out_fd != STDOUT_FILENO)
		close((*stack)->out_fd);
	close((*stack)->pipe[0]);
	close((*stack)->pipe[1]);
	if (!redir->next)
	{
		undo_duplicate_std(1);
		close_all_open_redir_ends(data);
		close_all_pipe_ends(stack);
		execve(cmd, (*stack)->node->argv, data->env);
		clean_execve_failure(data, stack);
	}
}

void	child_no_redir(t_data *data, t_stack **stack, char *cmd)
{
	int	cmd_i;

	cmd_i = get_first_command(data, stack);
	get_first_command(data, stack);
	if ((*stack)->in_fd == -1)
	{
		if ((*stack)->node->argv && !has_node_type_ancestor(*stack, NODE_PIPE)
			&& is_builtin_no_fork((*stack)->node->argv[cmd_i]))
			return ;
		executor_cleanup(data, stack, cmd);
		exit(1);
	}
	if ((*stack)->in_fd != STDIN_FILENO)
		dup2((*stack)->in_fd, STDIN_FILENO);
	if ((*stack)->in_fd != STDIN_FILENO)
		close((*stack)->in_fd);
	if ((*stack)->out_fd != STDOUT_FILENO)
		dup2((*stack)->out_fd, STDOUT_FILENO);
	if ((*stack)->out_fd != STDOUT_FILENO)
		close((*stack)->out_fd);
	child_execute(data, stack, cmd);
}

void	clean_execve_failure(t_data *data, t_stack **stack)
{
	int		exit_status;
	char	*sh_argv[3];

	sh_argv[0] = "sh";
	sh_argv[1] = (*stack)->node->argv[get_first_command(data, stack)];
	sh_argv[2] = NULL;
	exit_status = 126;
	if (errno == ENOEXEC)
		execve("/bin/sh", sh_argv, data->env);
	if (errno == ENOENT)
		exit_status = 127;
	system_error(strerror(errno), (*stack)->node->argv[
		get_first_command(data, stack)]);
	executor_cleanup(data, stack, NULL);
	exit(exit_status);
}
