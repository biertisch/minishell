/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:39:13 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/01 12:39:13 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_unset(t_data *data, t_stack **stack)
{
	int	valid;
	int	i;

	valid = check_unset_input(stack);
	if (valid)
	{
		
		if (!has_node_type_ancestor(*stack, NODE_PIPE) && !has_node_type_ancestor(*stack, NODE_SUBSHELL))
		{
			(*stack)->exit_status = valid;
			return (valid);
		}
		else
			exit(valid);
	}
	i = 1;
	while ((*stack)->node->argv[i])
		unset_env(&((data)->env_list), (*stack)->node->argv[i++]);
	if (has_node_type_ancestor(*stack, NODE_PIPE) || has_node_type_ancestor(*stack, NODE_SUBSHELL))
		exit(0);
	(*stack)->exit_status = 0;
	return (0);
}

int	check_unset_input(t_stack **stack)
{
	int	i;

	i = 1;
	while ((*stack)->node->argv[i] && is_valid_var_name((*stack)->node->argv[i]))
		i++;
	if ((*stack)->node->argv[i] && is_valid_var_name((*stack)->node->argv[i]))
		return (0);
	if (*((*stack)->node->argv[1]) == '-')
		return (2);
	return (1);
}
