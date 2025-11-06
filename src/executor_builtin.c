/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:31:38 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:10:27 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
		return (execute_builtin_entered(data, stack));
	if ((*stack)->phase == DONE)
		return (execute_builtin_done(&data, stack));
	return (0);
}

int	execute_builtin_entered(t_data *data, t_stack **stack)
{
	pid_t	pid;
	int		cmd_i;

	cmd_i = get_first_command(data, stack);
	(*stack)->phase = DONE;
	//if (!validate_builtin(data, (*stack)->node, cmd_i))
//	{
		if (execute_builtin_should_run_child(data, stack, cmd_i))
		{
			pid = fork();
			if (pid < 0)
				return (validate_fork(data, stack));
			if (pid == 0)
				child(data, stack);
			else
				parent(stack, pid);
		}
//	}
	return (0);
}

int	execute_builtin_should_run_child(t_data *data, t_stack **stack, int cmd_i)
{
	if (has_node_type_ancestor(*stack, NODE_PIPE))
		return (1);
	if (!has_node_type_ancestor(*stack, NODE_SUBSHELL)
		&& !ft_strcmp((*stack)->node->argv[cmd_i], "cd"))
		return (execute_cd(data, stack), 0);
	else if (!ft_strcmp((*stack)->node->argv[cmd_i], "exit"))
		return (execute_exit(data, stack), 0);
	else if (!ft_strcmp((*stack)->node->argv[cmd_i], "unset"))
		return (execute_unset(data, stack), 0);
	else if (!ft_strcmp((*stack)->node->argv[cmd_i], "export"))
		return (execute_export(data, stack), 0);
	return (1);
}

int	execute_builtin_done(t_data **data, t_stack **stack)
{
	if ((*stack)->next)
		setup_next_to_top(data, stack);
	else if (stack_size(*stack) == 1 || !get_first_pipe(stack))
		(*data)->exit_status = (*stack)->exit_status;
	pop(stack);
	return (1);
}

int	choose_and_execute_builtin(t_data *data, t_stack **stack)
{
	if (!ft_strcmp((*stack)->node->argv[0], "echo"))
		execute_echo(data, stack);
	if (!ft_strcmp((*stack)->node->argv[0], "env"))
		execute_env(data, stack);
	if (!ft_strcmp((*stack)->node->argv[0], "cd"))
		execute_cd(data, stack);
	if (!ft_strcmp((*stack)->node->argv[0], "pwd"))
		execute_pwd(data, stack);
	if (!ft_strcmp((*stack)->node->argv[0], "exit"))
		execute_exit(data, stack);
	if (!ft_strcmp((*stack)->node->argv[0], "unset"))
		execute_unset(data, stack);
	if (!ft_strcmp((*stack)->node->argv[0], "export"))
		execute_export(data, stack);
	return (0);
}
