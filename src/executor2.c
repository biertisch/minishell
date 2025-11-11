/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:47:06 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 11:47:07 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cmd_no_cmd(t_data *data, t_stack **stack)
{
	pid_t	pid;

	if (has_node_type_ancestor(*stack, NODE_PIPE))
	{
		pid = fork();
		if (pid == 0)
		{
			close_all_pipe_ends(stack);
			free_stack(stack);
			free_all(data);
			exit(0);
		}
		else
			parent(stack, pid);
	}
	cmd_has_variable(data, stack);
	pop(stack);
	return (1);
}

int	execute_cmd_pre_processing(t_data *data, t_stack **stack)
{
	if (expand_argv(data, (*stack)->node))
	{
		if ((*stack)->next)
			(*stack)->next->exit_status = 1;
		pop(stack);
		return (1);
	}
	if ((*stack)->node->argv && !(*stack)->node->argv[
			get_first_command(data, stack)])
	{
		execute_cmd_no_cmd(data, stack);
		return (1);
	}
	execute_export_handle_underscore(data, stack);
	return (0);
}
