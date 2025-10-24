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
	sort_env(&data);
	if (!(*stack)->node->argv[1])
		execute_export_no_option(data, stack);
	else
		execute_export_option(data, stack);
	return (0);
}

int	execute_export_option(t_data *data, t_stack **stack)
{
	t_env	**env;
	int	i;
	char	**kv_split;
	int	found;

	i = get_first_command(data, stack) + 1;
	while ((*stack)->node->argv[i])
	{
		found = 0;
		env = &(data->env_list);
		kv_split = ft_split((*stack)->node->argv[i], '=');
		if (is_valid_var_name(kv_split[0]))
		{
			while (env && *env)
			{
				if (!ft_strcmp((*env)->key, kv_split[0]))
				{
					found = 1;
					(*env)->exported = 1;
					if (kv_split[1])
					{
						if ((*env)->value)
							free((*env)->value);
						(*env)->value = ft_strdup(kv_split[1]);
					}
					break ;
				}
				env = &(*env)->next;
			}
			if (!found)
				add_env_node(&data->env_list, create_env_node(ft_strdup(kv_split[0]), ft_strdup(kv_split[1]), 1));
		}
		else
		{
			write(STDERR_FILENO, "minishell: export: `", 20);
			write(STDERR_FILENO, (*stack)->node->argv[i], ft_strlen((*stack)->node->argv[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 26);
		}
			i++;
		ft_splitfree(kv_split);
	}
	return (0);
}

int	execute_export_no_option(t_data *data, t_stack **stack)
{
	t_env	*env;
	
	(void)stack;
	env = data->env_list;
	while (env)
	{
		if (env->exported)
		{
			write((*stack)->out_fd, "declare -x ", 11);
			write((*stack)->out_fd, env->key, ft_strlen(env->key));
			if (env->value && ft_strcmp(env->value, ""))
			{
				write((*stack)->out_fd, "=\"", 2);
				write((*stack)->out_fd, env->value, ft_strlen(env->value));
				write((*stack)->out_fd, "\"", 1);
			}
			write((*stack)->out_fd, "\n", 1);
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
					i = (*env)->exported;
					(*env)->exported = (*env)->next->exported;
					(*env)->next->exported = i;

				}
			}
			env = &(*env)->next;
		}
	}
}
