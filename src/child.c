/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:49:31 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/04 12:49:32 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child(t_data *data, t_stack **stack)
{
	char	*full_path;
	char	*cmd;
	t_redir	*redir;


	env_list_to_array(data);
	setup_signals_child(data);
//	check_for_variables(data, stack);
	cmd = NULL;
	if ((*stack)->type == NODE_CMD && (*stack)->node->argv)
	{
		full_path = correct_path(data, stack,(*stack)->node->argv[0]);
		(*stack)->node->argv[0] = full_path;
		cmd = ft_strdup((*stack)->node->argv[0]);
		free((*stack)->node->argv[0]);
		(*stack)->node->argv[0] = ft_strdup(ft_strrchr(cmd, '/') + 1);
	}
	redir = (*stack)->node->redir;
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

void	child_redir_in(t_data *data, t_stack **stack, char *cmd, t_redir *redir)
{
	redir->fd = open(redir->file, O_RDONLY);
	if (redir->fd != -1)
		dup2(redir->fd, STDIN_FILENO);
	if (redir->fd != -1)
		close(redir->fd);
	else
	{
		if (errno == ENOENT)
		{
			write(STDERR_FILENO, (*stack)->node->argv[0], ft_strlen((*stack)->node->argv[0]));
			write(STDERR_FILENO, ": ", 2);
			write(STDERR_FILENO, redir->file, ft_strlen(redir->file));
			write(STDERR_FILENO, ": No such file or directory\n", 28);
		}
		free(cmd);
		free_stack(stack);
		free_all(data);
		exit(1);
	}
	if ((*stack)->out_fd != STDOUT_FILENO)
		dup2((*stack)->out_fd, STDOUT_FILENO);
	if ((*stack)->out_fd != STDOUT_FILENO)
		close((*stack)->out_fd);
	if (!redir->next && redir->fd != -1)
	{
		close_all_pipe_ends(stack);
		if ((*stack)->type == NODE_CMD)
		{
			execve(cmd, (*stack)->node->argv, data->env);
			clean_execve_failure(data, stack, cmd);
		}
		else
		{
			(*stack)->in_fd = STDIN_FILENO;
			(*stack)->out_fd = STDOUT_FILENO;
			choose_and_execute_builtin(data, stack);
		}
	}
}

void	child_redir_out(t_data *data, t_stack **stack, char *cmd, t_redir *redir)
{
	if (redir->type == REDIR_OUT)
		redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	dup2(redir->fd, STDOUT_FILENO);
	close(redir->fd);
	if ((*stack)->in_fd != STDIN_FILENO)
		dup2((*stack)->in_fd, STDIN_FILENO);
	if ((*stack)->in_fd != STDIN_FILENO)
		close((*stack)->in_fd);
	if (!redir->next)
	{
		close_all_pipe_ends(stack);
		if ((*stack)->type == NODE_CMD)
		{
			close_all_pipe_ends(stack);
			execve(cmd, (*stack)->node->argv, data->env);
			clean_execve_failure(data, stack, cmd);
		}
		else
		{
			(*stack)->in_fd = STDIN_FILENO;
			(*stack)->out_fd = STDOUT_FILENO;
			choose_and_execute_builtin(data, stack);
		}
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
		close_all_pipe_ends(stack);
		execve(cmd, (*stack)->node->argv, data->env);
		clean_execve_failure(data, stack, cmd);
	}
}

void	child_no_redir(t_data *data, t_stack **stack, char *cmd)
{
	if ((*stack)->in_fd != STDIN_FILENO)
		dup2((*stack)->in_fd, STDIN_FILENO);
	if ((*stack)->in_fd != STDIN_FILENO)
		close((*stack)->in_fd);
	if ((*stack)->out_fd != STDOUT_FILENO)
		dup2((*stack)->out_fd, STDOUT_FILENO);
	if ((*stack)->out_fd != STDOUT_FILENO)
		close((*stack)->out_fd);
	close_all_pipe_ends(stack);
	if ((*stack)->type == NODE_CMD)
	{
		execve(cmd, (*stack)->node->argv, data->env);
		clean_execve_failure(data, stack, cmd);
	}
	else
	{
		(*stack)->in_fd = STDIN_FILENO;
		(*stack)->out_fd = STDOUT_FILENO;
		choose_and_execute_builtin(data, stack);
	}
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
	free(cmd);
	free_stack(stack);
	free_all(data);
	exit (exit_status);
}
