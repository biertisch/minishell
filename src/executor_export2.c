/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_export2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:58:37 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 10:58:37 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_export_val_not_found(t_data *data, t_stack **stack, char **kv_split)
{
	char	*str1;
	char	*str2;

	str1 = ft_strdup(kv_split[0]);
	str2 = ft_strdup(kv_split[1]);
	if (!str1 || (kv_split[1] && !str2))
	{
		ft_splitfree(kv_split);
		if (!str1)
			validate_malloc_execute(data, stack, str1, str2);
		else
			validate_malloc_execute(data, stack, str2, str1);
	}
	add_env_node(&data->env_list, create_env_node(str1, str2, 1));
	return (1);
}

int	execute_export_no_option(t_data *data)
{
	t_env	*env;

	env = data->env_list;
	while (env)
	{
		if (env->exported)
		{
			write(STDOUT_FILENO, "declare -x ", 11);
			write(STDOUT_FILENO, env->key, ft_strlen(env->key));
			if (env->value && ft_strcmp(env->value, ""))
			{
				write(STDOUT_FILENO, "=\"", 2);
				write(STDOUT_FILENO, env->value, ft_strlen(env->value));
				write(STDOUT_FILENO, "\"", 1);
			}
			write(STDOUT_FILENO, "\n", 1);
		}
		env = env->next;
	}
	return (0);
}
