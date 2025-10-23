/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 12:36:12 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/19 12:36:13 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_if_variable(t_data *data, t_stack **stack)
{
	int	i;
	char	**kv_split;
	t_env	**env;
	int	found;

	i = 0;
	while ((*stack)->node->argv[i])
	{
		kv_split = ft_split((*stack)->node->argv[i], '=');
		if (is_valid_var_name(kv_split[0]) && kv_split[1])
		{
			env = &(data->env_list);
			found = 0;
			while (env && *env)
			{
				if (!ft_strcmp((*env)->key, kv_split[0]))
				{
					found = 1;
					if ((*env)->value)
						free((*env)->value);
					(*env)->value = ft_strdup(kv_split[1]);
				}
				env = &(*env)->next;
			}
			if (!found)
				add_env_node(&data->env_list, create_env_node(ft_strdup(kv_split[0]), ft_strdup(kv_split[1]), 0));
		}
		else
			return (ft_splitfree(kv_split), 0);
		ft_splitfree(kv_split);
		i++;
	}
	return (1);
}

int	get_first_command(t_data *data, t_stack **stack)
{
	int	i;
	char	**kv_split;

	(void)data;
	i = 0;
	while ((*stack)->node->argv[i])
	{
		kv_split = ft_split((*stack)->node->argv[i], '=');
		if (!is_valid_var_name(kv_split[0]) || !kv_split[1])
			return (ft_splitfree(kv_split), i);
		i++;
		ft_splitfree(kv_split);
	}
	return (i);
}

int	check_if_variables_with_commands(t_data *data, t_stack **stack)
{
	int	i;
	char	*key;

	i = 0;
	while ((*stack)->node->argv[i])
	{
		key = ft_strdup_n((*stack)->node->argv[i], ft_strlen((*stack)->node->argv[i]) - ft_strlen(ft_strchr((*stack)->node->argv[i], '=')));
		if (ft_strchr((*stack)->node->argv[i], '='))
		{
			if (get_env_value(data->env_list, key))
				set_env_value(data->env_list, key, ft_strdup(ft_strchr((*stack)->node->argv[i], '=') + 1));
			else 
				add_env_node(&(data->env_list), create_env_node(ft_strdup(key), ft_strdup(ft_strchr((*stack)->node->argv[i], '=') + 1), 1));
		}
		else
			return (free(key), i);
		free(key);
		i++;
	}
	return (i);
}
