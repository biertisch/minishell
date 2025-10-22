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
	t_env	*new_node;

	i = 0;
	if (has_command(data, stack))
		return (0);
	while ((*stack)->node->argv[i])
	{
		if (ft_strchr((*stack)->node->argv[i], '='))
		{
			new_node = create_env_node(ft_strdup_n((*stack)->node->argv[i], ft_strlen((*stack)->node->argv[i]) - ft_strlen(ft_strchr((*stack)->node->argv[i], '='))), ft_strdup(ft_strchr((*stack)->node->argv[i], '=') + 1), 0);
			add_env_node(&(data->env_list), new_node);
		}
		else
			return (0);
		i++;
	}
	return (1);
}

int	has_command(t_data *data, t_stack **stack)
{
	int	i;

	(void)data;
	i = 0;
	while ((*stack)->node->argv[i])
	{
		if (!ft_strchr((*stack)->node->argv[i], '='))
			return (1);
		i++;
	}
	return (0);
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
