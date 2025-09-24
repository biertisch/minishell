/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_subshell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:57:43 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/24 11:57:44 by pedde-so         ###   ########.fr       */
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
	pid_t	res;
	int		status;


	(*stack)->phase = DONE;
	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		push_stack(stack, (*stack)->node->left, STDIN_FILENO, STDOUT_FILENO, *data);
	}
	else
	{
		res = waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(*stack)->exit_status = WEXITSTATUS(status);
	}
	return (0);
}

int	execute_subshell_done(t_data **data, t_stack **stack)
{
	int	left_tree_size;

	if ((*stack)->next)
		setup_next_to_top(data, stack);
	else
		(*data)->exit_status = (*stack)->exit_status;
	left_tree_size = count_tree_nodes((*stack)->node->left);
	pop(stack);
	return (1 + left_tree_size);

}
