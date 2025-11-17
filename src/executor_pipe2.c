/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:52:48 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/17 13:55:45 by beatde-a         ###   ########.fr       */
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
		{
			if (WIFSIGNALED(status))
				g_sig = WTERMSIG(status);
		}
		if (g_sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		g_sig = 0;
	}
	if ((*stack)->next)
		setup_next_to_top(data, stack);
	else
		(*data)->exit_status = (*stack)->exit_status;
	pop(stack);
	return (1);
}
