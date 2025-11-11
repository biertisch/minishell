/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:52:48 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 11:52:49 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pipe_done(t_data **data, t_stack **stack)
{
	int	status;

	status = 0;
	if (!get_next_pipe_in_subshell(stack))
	{
		while (waitpid(-1, &status, 0) > 0)
			handle_child_exit(status);
	}
	if ((*stack)->next)
		setup_next_to_top(data, stack);
	else
		(*data)->exit_status = (*stack)->exit_status;
	pop(stack);
	return (1);
}
