/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 12:36:12 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/04 09:58:55 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_has_variable(t_data *data, t_stack **stack)
{
	char	**kv_split;
	int		i;

	if (!(*stack)->node->argv)
		return (0);
	if (!ft_strcmp((*stack)->node->argv[get_first_command(data, stack)], "cd"))
		return (0);
	i = 0;
	while ((*stack)->node->argv[i])
	{
		kv_split = split_by_first_equal((*stack)->node->argv[i]);
		validate_malloc_execute(data, stack, kv_split, NULL);
		if (!is_valid_var_name(kv_split[0]) || !kv_split[1])
			return (ft_splitfree(kv_split), 0);
		else
			cmd_has_valid_variable(data, stack, kv_split);
		ft_splitfree(kv_split);
		i++;
	}
	return (1);
}

void	cmd_has_valid_variable(t_data *data, t_stack **stack, char **kv_split)
{
	t_env	**env;

	env = &(data->env_list);
	while (*env && ft_strcmp((*env)->key, kv_split[0]))
		env = &(*env)->next;
	if (*env)
	{
		if ((*env)->value)
			free((*env)->value);
		(*env)->value = ft_strdup(kv_split[1]);
		if (!(*env)->value)
		{
			ft_splitfree(kv_split);
			validate_malloc_execute(data, stack, (*env)->value, NULL);
		}
	}
	else
		variable_key_not_found(data, stack, kv_split);
}

char	**split_by_first_equal(char *var)
{
	char	*eq;
	char	**res;

	eq = ft_strchr(var, '=');
	if (!eq)
		return (ft_split(var, '='));
	res = malloc(3 * sizeof(char *));
	if (!res)
		return (NULL);
	*res = ft_substr(var, 0, (size_t)(eq - var));
	if (!*res)
		return (free(res), NULL);
	*(res + 1) = ft_strdup(eq + 1);
	if (!*(res + 1))
		return (ft_splitfree_error(res, 1), NULL);
	*(res + 2) = NULL;
	return (res);
}

int	get_first_command(t_data *data, t_stack **stack)
{
	char	**kv_split;
	int		i;

	(void)data;
	if (!(*stack)->node->argv)
		return (-1);
	i = 0;
	while ((*stack)->node->argv[i])
	{
		kv_split = ft_split((*stack)->node->argv[i], '=');
		validate_malloc_execute(data, stack, kv_split, NULL);
		if (!is_valid_var_name(kv_split[0])
			|| (!kv_split[1] && !ft_strchr((*stack)->node->argv[i], '=')))
			return (ft_splitfree(kv_split), i);
		i++;
		ft_splitfree(kv_split);
	}
	return (i);
}

int	variable_key_not_found(t_data *data, t_stack **stack, char **kv_split)
{
	t_env	*new_node;
	char	*str1;
	char	*str2;

	str1 = ft_strdup(kv_split[0]);
	str2 = ft_strdup(kv_split[1]);
	if (!str1 || !str2)
	{
		ft_splitfree(kv_split);
		if (!str1)
			validate_malloc_execute(data, stack, str1, str2);
		else
			validate_malloc_execute(data, stack, str2, str1);
	}
	new_node = create_env_node(str1, str2, 0);
	if (!new_node)
	{
		free(str1);
		validate_malloc_execute(data, stack, new_node, str2);
	}
	add_env_node(&data->env_list, new_node);
	return (0);
}
