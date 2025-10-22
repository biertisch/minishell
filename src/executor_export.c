/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:18:10 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/22 17:18:12 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_export(t_data *data, t_stack **stack)
{
	if (!(*stack)->node->argv[1])
	{
		execute_export_no_option(data, stack);
		free_stack(stack);
		free_all(data);
		exit(0);
	}
	return (0);
}

int	execute_export_no_option(t_data *data, t_stack **stack)
{
	t_env	*env;
	
	(void)stack;
	sort_env(&data);
	env = data->env_list;
	while (env)
	{
		if (env->exported)
		{
			write(STDOUT_FILENO, "declare -x ", 11);
			write(STDOUT_FILENO, env->key, ft_strlen(env->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, env->value, ft_strlen(env->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		env = env->next;
	}
	return (0);
}

void	sort_env(t_data **data)
{
	int	sorted;
	t_env	**env;
	char	*aux;

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
					free((*env)->key);
					(*env)->key = ft_strdup((*env)->next->key);
					free((*env)->next->key);
					(*env)->next->key = ft_strdup(aux);
					free(aux);
					aux = ft_strdup((*env)->value);
					free((*env)->value);
					(*env)->value = ft_strdup((*env)->next->value);
					free((*env)->next->value);
					(*env)->next->value = ft_strdup(aux);
					free(aux);
				}
			}
			env = &(*env)->next;
		}
	}
}
