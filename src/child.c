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
	char	*temp;
	int		cmd_i;

	env_list_to_array(data);
	setup_signals_child(data);
	cmd_has_variable(data, stack);
	cmd_i = get_first_command(data, stack);
	cmd = NULL;
	if ((*stack)->node->argv && !is_builtin((*stack)->node->argv[0])
		&& !has_failed_redirect((*stack)->node->redir))
	{
		cmd = correct_path(data, stack, (*stack)->node->argv[cmd_i]);
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
		if ((redir->type == REDIR_OUT || redir->type == APPEND)
			&& redir->out_fd == -1)
			return (1);
		redir = redir->next;
	}
	return (0);
}

void	handle_redirects(t_data *data, t_stack **stack,
		char *cmd, t_redir *redir)
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

void	child_execute(t_data *data, t_stack **stack, char *cmd)
{
	close_all_pipe_ends(stack);
	check_no_cmd(data, stack);
	close_all_open_redir_ends(data);
	undo_duplicate_std(1);
	if (!is_builtin((*stack)->node->argv[0]))
	{
		execve(cmd, (*stack)->node->argv, data->env);
		clean_execve_failure(data, stack);
	}
	else if (has_node_type_ancestor(*stack, NODE_PIPE)
		|| !is_builtin_no_fork((*stack)->node->argv[0]))
		choose_and_execute_builtin(data, stack);
}
