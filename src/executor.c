/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:37:51 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/02 12:37:53 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute(t_data *data)
{
	t_stack	*stack;

	stack = create_stack(data);
	execute_stack(data, &stack);
	return (1);
}

int	execute_stack(t_data *data, t_stack **stack)
{
	int	i;
	int	tree_nodes_count;

	tree_nodes_count = count_tree_nodes(data->parser_tree);
	i = 0;
	while (i < tree_nodes_count)
	{
		if ((*stack)->type == NODE_PIPE)
			i += execute_pipe(data, stack);
		else if ((*stack)->type == NODE_CMD)
			i += execute_cmd(data, stack);
	}
	pop(stack);
	return (0);
}

int	execute_cmd(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
		return (execute_cmd_entered(data, stack));
	if ((*stack)->phase == DONE)
		return (execute_cmd_done(stack));
	return (0);
}

int	execute_cmd_entered(t_data *data, t_stack **stack)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		check_for_errors(-1, data, *stack, "fork");
	else if (pid == 0)
		child(data, stack);
	else
		parent(stack, pid);
	(*stack)->phase = DONE;
	return (0);
}

int	execute_cmd_done(t_stack **stack)
{
	pop(stack);
	return (1);
}
