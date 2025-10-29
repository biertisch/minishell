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
	t_env **first;
	t_env **second;
	t_env **third;
	t_env	*victim;
	t_env	*next;
	int	i;

	i = get_first_command(data, stack) + 1;
	while ((*stack)->node->argv[i])
	{
		first = NULL;
		second = &(data->env_list);
		third = NULL;
		victim = NULL;
		next = NULL;
		while (second && *second)
		{
			third = &((*second)->next);
			if (!ft_strcmp((*second)->key, (*stack)->node->argv[i]))
			{
				victim = *second;
				next = *third;
				if (first)
					(*first)->next = next;
				else	
					data->env_list = next;
				free_env_node(&victim);
				break ;
			}
			first = second;
			second = third;
			third = NULL;
		}
		i++;
	}
	return (0);
}
