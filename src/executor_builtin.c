/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:26:26 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/23 13:26:28 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

	pid = fork();
	if (pid == 0)
		child(data, stack);
	else
		parent(stack, pid);
	(*stack)->phase = DONE;
	return (0);
}

int	execute_builtin_done(t_data **data, t_stack **stack)
{
	if (stack_size(*stack) == 1 || !get_first_pipe(stack))
		(*data)->exit_status = (*stack)->exit_status;
	pop(stack);
	return (1);

}

int	choose_and_execute_builtin(t_data *data, t_stack **stack)
{
	if (!ft_strcmp((*stack)->node->argv[0], "echo"))
		execute_echo(data, stack);
	return (0);
}
