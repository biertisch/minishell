/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:18:10 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:10:57 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_export(t_data *data, t_stack **stack)
{
	int	cmd_i;
	
	cmd_i = get_first_command(data, stack);
	duplicate_std();	
	if (!has_node_type_ancestor(*stack, NODE_PIPE))
		handle_redirects(data, stack, NULL, (*stack)->node->redir);
	sort_env(&data);
	if (!(*stack)->node->argv[cmd_i + 1])
		execute_export_no_option(data, stack);
	else
		execute_export_option(data, stack, cmd_i);
	undo_duplicate_std(1);
	execute_builtin_check_for_pipe(data, stack);
	return (0);
}

int	execute_export_option(t_data *data, t_stack **stack, int cmd_i)
{
	t_env	**env;
	char	**kv_split;

	while ((*stack)->node->argv[cmd_i + 1])
	{
		kv_split = ft_split((*stack)->node->argv[cmd_i + 1], '=');
		validate_malloc_execute(data, stack, kv_split, NULL);
		if (*((*stack)->node->argv[cmd_i + 1]) != '=' && is_valid_var_name(kv_split[0]))
		{
			env = &(data->env_list);
			while (env && *env && ft_strcmp((*env)->key, kv_split[0]))
				env = &(*env)->next;
			if (env && *env)
				execute_export_val_found(data, stack, kv_split, env);
			else
				execute_export_val_not_found(data, stack, kv_split);
		}
		else
			execute_export_invalid_var(stack, cmd_i);
		ft_splitfree(kv_split);
		cmd_i++;
	}
	return (0);
}

int	execute_export_val_found(t_data *data, t_stack **stack,
			char **kv_split, t_env **env)
{
	(*env)->exported = 1;
	if (kv_split[1])
	{
		if ((*env)->value)
			free((*env)->value);
		(*env)->value = ft_strdup(kv_split[1]);
		if (!(*env)->value)
		{
			ft_splitfree(kv_split);
			validate_malloc_execute(data, stack, NULL, NULL);
		}
	}
	return (1);
}

void	execute_export_handle_underscore(t_data *data, t_stack **stack)
{
	char	*value;
	int	i;

	if (!(*stack)->node->argv)
		return ;
	i = 0;
	while ((*stack)->node->argv[i])
		i++;
	value = ft_strdup((*stack)->node->argv[i - 1]);
	validate_malloc_execute(data, stack, value, NULL);
	set_env_value(data->env_list, "_", value);
}

void	execute_export_invalid_var(t_stack **stack, int cmd_i)
{
	write(STDERR_FILENO, "minishell: export: `", 20);
	write(STDERR_FILENO, (*stack)->node->argv[cmd_i + 1],
		ft_strlen((*stack)->node->argv[cmd_i + 1]));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
	(*stack)->exit_status = 1;
}

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

int	execute_export_no_option(t_data *data, t_stack **stack)
{
	t_env	*env;

	(void)stack;
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
