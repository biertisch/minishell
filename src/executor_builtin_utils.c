/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 11:53:30 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/04 11:53:32 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*duplicate_std(void)
{
	static int	dups[6];

	if (dups[1] == 0)
	{
		dups[0] = dup(STDIN_FILENO);
		if (dups[0] < 0)
			return (dups);
		dups[1] = 1;
	}
	if (dups[3] == 0)
	{
		dups[2] = dup(STDOUT_FILENO);
		if (dups[2] < 0)
			return (dups);
		dups[3] = 1;
	}
	if (dups[5] == 0)
	{
		dups[4] = dup(STDERR_FILENO);
		if (dups[4] < 0)
			return (dups);
		dups[5] = 1;
	}
	return (dups);
}

void	undo_duplicate_std(int dup)
{
	int	*dups;

	dups = duplicate_std();
	if (dups[1] != 0)
	{
		if (dup)
			dup2(dups[0], STDIN_FILENO);
		close(dups[0]);
		dups[1] = 0;
	}
	if (dups[3] != 0)
	{
		if (dup)
			dup2(dups[2], STDOUT_FILENO);
		close(dups[2]);
		dups[3] = 0;
	}
	if (dups[5] != 0)
	{
		if (dup)
			dup2(dups[4], STDERR_FILENO);
		close(dups[4]);
		dups[5] = 0;
	}
}

void	execute_builtin_check_for_pipe(t_data *data, t_stack **stack)
{
	int	exit_status;

	if (has_node_type_ancestor(*stack, NODE_PIPE))
	{
		exit_status = (*stack)->exit_status;
		close_all_pipe_ends(stack);
		close_all_open_redir_ends(data);
		free_stack(stack);
		free_all(data);
		exit(exit_status);
	}
}
