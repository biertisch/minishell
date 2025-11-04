/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 12:36:12 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:16:44 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_has_variable(t_data *data, t_stack **stack)
{
	int	i;
	char	**kv_split;
	t_env	**env;
	int	found;

	if (!(*stack)->node->argv)
		return (0);
	if (!ft_strcmp((*stack)->node->argv[get_first_command(data, stack)], "cd"))
		return (0);
	i = 0;
	while ((*stack)->node->argv[i])
	{
		kv_split = split_by_first_equal((*stack)->node->argv[i]);
		validate_malloc_execute(data, stack, kv_split, NULL);
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
					if (!(*env)->value)
					{
						ft_splitfree(kv_split);
						validate_malloc_execute(data, stack, (*env)->value, NULL);
					}
				}
				env = &(*env)->next;
			}
			if (!found)
				variable_key_not_found(data, stack, kv_split);
		}
		else
			return (ft_splitfree(kv_split), 0);
		ft_splitfree(kv_split);
		i++;
	}
	return (1);
}

char	**split_by_first_equal(char *var)
{
	char	**res;
	char	*first_eq;
	int		i;
	int		j;

	first_eq = ft_strchr(var, '=');
	if (!first_eq)
		return (ft_split(var, '='));
	res = malloc(3 * sizeof(char *));
	if (!res)
		return (NULL);
	res[0] = malloc(first_eq - var + 1);
	if (!res[0])
		return (free(res), NULL);
	res[1] = malloc(ft_strlen(var) - ft_strlen(first_eq) + 1);
	if (!res[1])
		return (ft_splitfree_error(res, 1), NULL);
	i = -1;
	while (++i < (first_eq - var))
		res[0][i] = *(var + i);
	res[0][i] = '\0';
	j = 0;
	while (++i < (int)ft_strlen(var))
		res[1][j++] = *(var + i);
	res[1][j] = '\0';
	res[2] = NULL;
	return (res);
}

int	get_first_command(t_data *data, t_stack **stack)
{
	int	i;
	char	**kv_split;

	(void)data;
	if (!(*stack)->node->argv)
		return (-1);
	i = 0;
	while ((*stack)->node->argv[i])
	{
		kv_split = ft_split((*stack)->node->argv[i], '=');
		validate_malloc_execute(data, stack, kv_split, NULL);
		if (!is_valid_var_name(kv_split[0]) || !kv_split[1])
			return (ft_splitfree(kv_split), i);
		i++;
		ft_splitfree(kv_split);
	}
	return (i);
}

int	variable_key_not_found(t_data *data, t_stack **stack, char **kv_split)
{
	char		*str1;
	char		*str2;
	t_env	*new_node;

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
