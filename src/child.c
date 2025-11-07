/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:49:31 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:09:52 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child(t_data *data, t_stack **stack)
{
	char	*cmd;
	int	cmd_i;
	char	*temp;


	env_list_to_array(data);
	setup_signals_child(data);
	cmd_has_variable(data, stack);
	cmd_i = get_first_command(data, stack);
	cmd = NULL;
	if ((*stack)->node->argv && !is_builtin((*stack)->node->argv[0])
		&& !has_failed_redirect((*stack)->node->redir))
	{
		cmd = correct_path(data, stack, (*stack)->node->argv[cmd_i]);
		//here
		//do with other cmds?
		if (ft_strcmp("/bin/echo", cmd))
		{
			temp = ft_strdup(ft_strrchr(cmd, '/') + 1);
			validate_malloc_execute(data, stack, temp, cmd);
			free((*stack)->node->argv[cmd_i]);
			(*stack)->node->argv[cmd_i] = temp;
			
		}
	}
	handle_redirects(data, stack, cmd, (*stack)->node->redir);
}

int	has_failed_redirect(t_redir *redir)
{
	if (!redir)
		return (0);
	while (redir)
	{
		if (redir->type == REDIR_IN && redir->in_fd == -1)
			return (1);
		if ((redir->type == REDIR_OUT || redir->type == APPEND) && redir->out_fd == -1)
			return (1);
		redir = redir->next;
	}
	return (0);
}

void	handle_redirects(t_data *data, t_stack **stack, char *cmd, t_redir *redir)
{
	if (!redir)
		child_no_redir(data, stack, cmd);
	while (redir)
	{
		if (redir->type == REDIR_IN)
			child_redir_in(data, stack, cmd, redir);
		else if (redir->type == HEREDOC)
			child_heredoc(data, stack, cmd, redir);
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
			child_redir_out(data, stack, cmd, redir);
		redir = redir->next;
	}
}

void	check_no_cmd(t_data *data, t_stack **stack)
{
	if (!(*stack)->node->argv)
	{
		executor_cleanup(data, stack, NULL);
		exit(0);
	}
}

void	child_redir_in(t_data *data, t_stack **stack, char *cmd, t_redir *redir)
{
	if (redir->in_fd == -1)
	{
		if ((*stack)->node->argv && !has_node_type_ancestor(*stack, NODE_PIPE) && is_builtin_no_fork((*stack)->node->argv[0]))
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
	{
		close_all_pipe_ends(stack);
		check_no_cmd(data, stack);
		close_all_open_redir_ends(data);
		undo_duplicate_std(0);
		if (!is_builtin((*stack)->node->argv[0]))
		{
			execve(cmd, (*stack)->node->argv, data->env);
			clean_execve_failure(data, stack, cmd);
		}
		else if (has_node_type_ancestor(*stack, NODE_PIPE)
			|| !is_builtin_no_fork((*stack)->node->argv[0]))
			choose_and_execute_builtin(data, stack);
	}
}

void	child_redir_out(t_data *data, t_stack **stack, char *cmd, t_redir *redir)
{
	if (redir->out_fd == -1)
	{
		if ((*stack)->node->argv && !has_node_type_ancestor(*stack, NODE_PIPE) && is_builtin_no_fork((*stack)->node->argv[0]))
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
	{
		close_all_pipe_ends(stack);
		check_no_cmd(data, stack);
		close_all_open_redir_ends(data);
		undo_duplicate_std(0);
		if ((*stack)->node->argv && !is_builtin((*stack)->node->argv[0]))
		{
			execve(cmd, (*stack)->node->argv, data->env);
			clean_execve_failure(data, stack, cmd);
		}
		else if (has_node_type_ancestor(*stack, NODE_PIPE)
			|| !is_builtin_no_fork((*stack)->node->argv[0]))
			choose_and_execute_builtin(data, stack);
	}
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
		undo_duplicate_std(0);
		close_all_open_redir_ends(data);
		close_all_pipe_ends(stack);
		execve(cmd, (*stack)->node->argv, data->env);
		clean_execve_failure(data, stack, cmd);
	}
}

void	child_no_redir(t_data *data, t_stack **stack, char *cmd)
{
	int cmd_i;

	cmd_i = get_first_command(data, stack);
	if ((*stack)->in_fd == -1)
	{
		if ((*stack)->node->argv && !has_node_type_ancestor(*stack, NODE_PIPE) && is_builtin_no_fork((*stack)->node->argv[0]))
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
	close_all_pipe_ends(stack);
	check_no_cmd(data, stack);
	close_all_open_redir_ends(data);
	undo_duplicate_std(0);
	if (!is_builtin((*stack)->node->argv[0]))
	{
		close_all_open_redir_ends(data);
		execve(cmd, (*stack)->node->argv + cmd_i, data->env);
		clean_execve_failure(data, stack, cmd);
	}
	else if (has_node_type_ancestor(*stack, NODE_PIPE)
		|| !is_builtin_no_fork((*stack)->node->argv[cmd_i]))
		choose_and_execute_builtin(data, stack);
}


void	clean_execve_failure(t_data *data, t_stack **stack, char *cmd)
{
	int		exit_status;
	char	*sh_argv[3];

	sh_argv[0] = "sh";
	sh_argv[1] = (*stack)->node->argv[0];
	sh_argv[2] = NULL;
	exit_status = 126;
	if (errno != ENOEXEC)
		write(STDERR_FILENO, (*stack)->node->argv[0], ft_strlen((*stack)->node->argv[0]));
	if (errno == ENOENT)
	{
		write(STDERR_FILENO, ": command not found\n", 20);
		exit_status = 127;
	}
	else if (errno == EACCES || errno == EISDIR)
		write(STDERR_FILENO, ": Permission denied\n", 20);
	else if (errno == ENOEXEC)
	{
		execve("/bin/sh", sh_argv, data->env);
		write(STDERR_FILENO, (*stack)->node->argv[0], ft_strlen((*stack)->node->argv[0]));
		write(STDERR_FILENO, ": Exec format error\n", 20);
	}
	else
		perror((*stack)->node->argv[0]);
	(void)cmd;
	undo_duplicate_std(0);
	free_stack(stack);
	free_all(data);
	exit (exit_status);
}
