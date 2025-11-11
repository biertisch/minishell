/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_subshell2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 16:28:28 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 16:28:29 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_subshell_done(t_data **data, t_stack **stack)
{
	int	tree_size;
	int	exit_status;

	if ((*stack)->child_count == -42)
	{
		exit_status = (*stack)->exit_status;
		close_all_pipe_ends(stack);
		free_all(*data);
		free_stack(stack);
		exit(exit_status);
	}
	tree_size = count_tree_nodes((*stack)->node);
	if ((*stack)->next)
		setup_next_to_top(data, stack);
	else
		(*data)->exit_status = (*stack)->exit_status;
	pop(stack);
	return (tree_size);
}
