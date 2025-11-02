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

void	sort_env(t_data **data, t_stack **stack)
{
	int	sorted;
	t_env	**env;
	char	*aux;
	int	i;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		env = &(*data)->env_list;
		while (env && *env)
		{
			if ((*env)->next)
			{
				if (ft_strcmp((*env)->key, (*env)->next->key) > 0)
				{
					sorted = 0;
					aux = ft_strdup((*env)->key);
					validate_malloc_execute(*data, stack, aux, NULL);
					free((*env)->key);
					(*env)->key = ft_strdup((*env)->next->key);
					validate_malloc_execute(*data, stack, (*env)->key, aux);
					free((*env)->next->key);
					(*env)->next->key = ft_strdup(aux);
					validate_malloc_execute(*data, stack, (*env)->next->key, aux);
					free(aux);
					aux = ft_strdup((*env)->value);
					validate_malloc_execute(*data, stack, aux, NULL);
					free((*env)->value);
					(*env)->value = ft_strdup((*env)->next->value);
					validate_malloc_execute(*data, stack, (*env)->value, aux);
					free((*env)->next->value);
					(*env)->next->value = ft_strdup(aux);
					validate_malloc_execute(*data, stack, (*env)->next->value, aux);
					free(aux);
					i = (*env)->exported;
					(*env)->exported = (*env)->next->exported;
					(*env)->next->exported = i;
				}
			}
			env = &(*env)->next;
		}
	}
}
