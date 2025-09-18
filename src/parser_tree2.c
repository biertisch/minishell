/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 11:51:22 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/18 11:51:35 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	push_right_once(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return ;
	if (data->stack->node->right)
		push_stack(&data->stack, data->stack->node->right, 0, 0, data);
	return ;
}

int	push_left_until_cmd(t_data *data, int (*f_sub)(t_data *, t_tree *))
{
	if (!data->stack || !data->stack->node)
		return (-1); //issue warning?
	while (data->stack->node->left)
	{
		if (data->stack->node->type == NODE_SUBSHELL)
		{
			if (f_sub)
				if (f_sub(data, data->stack->node))
					return (-1);
			data->stack->phase = DONE;
		}
		push_stack(&data->stack, data->stack->node->left, 0, 0, data);
	}
	return (0);
}
