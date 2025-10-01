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
	int	i;
	if (check_unset_input(stack))
		return ((*stack)->exit_status);
	i = 1;
	while ((*stack)->node->argv[i])
		unset_env(&((data)->env_list), (*stack)->node->argv[i++]);
	if (has_pipe_ancestor(*stack) || has_subshell_ancestor(*stack))
		exit(0);
	(*stack)->exit_status = 0;
	return (0);
}

int	check_unset_input(t_stack **stack)
{
	if (*((*stack)->node->argv[1]) == '-')
	{
		if (!has_pipe_ancestor(*stack) && !has_subshell_ancestor(*stack))
		{
			(*stack)->exit_status = 2;
			return (2);
		}
		else
			exit(2);
	}
	//check if is valid NAME
	return (0);
}
