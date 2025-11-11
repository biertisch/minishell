/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_sort_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 15:07:07 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/02 15:07:16 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_env(t_data **data)
{
	int		sorted;
	t_env	**env;
	t_env	*a;
	t_env	*b;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		env = &(*data)->env_list;
		while (env && *env && (*env)->next)
		{
			a = *env;
			b = a->next;
			if (ft_strcmp(a->key, b->key) > 0)
			{
				sorted = 0;
				a->next = b->next;
				b->next = a;
				*env = b;
			}
			else
				env = &(*env)->next;
		}
	}
}
