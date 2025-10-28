/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:51:22 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 16:55:27 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	push_left_until_cmd(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (0);
	while (data->stack->node->left)
	{
		if (data->stack->node->type == NODE_SUBSHELL)
			data->stack->phase = DONE;
		push_stack(&data->stack, data->stack->node->left, 0, 0, data);
	}
	return (0);
}

int	count_tree_nodes(t_tree *root)
{
	if (!root)
		return (0);
	return (1 + count_tree_nodes(root->left) + count_tree_nodes(root->right));
}
