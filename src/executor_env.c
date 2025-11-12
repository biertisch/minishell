/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:39:33 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:10:42 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_env(t_data *data, t_stack **stack)
{
	t_env	*env;
	int		exit_status;
	int		cmd_i;

	cmd_i = get_first_command(data, stack);
	exit_status = is_env_option(stack, cmd_i);
	pre_print_env_args(stack, exit_status, cmd_i);
	env = data->env_list;
	while (!exit_status && env)
	{
		if (env->key && ft_strcmp(env->key, "") && env->exported)
		{
			write(STDOUT_FILENO, env->key, ft_strlen(env->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, env->value, ft_strlen(env->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		env = env->next;
	}
	print_env_args(stack, exit_status, cmd_i);
	executor_cleanup(data, stack, NULL);
	exit(exit_status);
	return (1);
}

void	pre_print_env_args(t_stack **stack,
		int exit_status, int cmd_i)
{
	int	i;

	if (exit_status)
		return ;
	i = 0;
	while (i < cmd_i)
	{
		write(STDOUT_FILENO, (*stack)->node->argv[i],
			ft_strlen((*stack)->node->argv[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

void	print_env_args(t_stack **stack,
		int exit_status, int cmd_i)
{
	if (exit_status)
		return ;
	while ((*stack)->node->argv[++cmd_i])
	{
		write(STDOUT_FILENO, (*stack)->node->argv[cmd_i],
			ft_strlen((*stack)->node->argv[cmd_i]));
		write(STDOUT_FILENO, "\n", 1);
	}
}

int	is_env_option(t_stack **stack, int cmd_i)
{
	int	exit_status;
	int	i;

	cmd_i++;
	exit_status = 0;
	if ((*stack)->node->argv[cmd_i] && *(*stack)->node->argv[cmd_i] == '-')
	{
		internal_error(INT_ERR_9, "env", (*stack)->node->argv[cmd_i]);
		exit_status = 125;
	}
	else if ((*stack)->node->argv[cmd_i])
	{
		i = 0;
		while ((*stack)->node->argv[cmd_i + i] && !exit_status)
		{
			if (!ft_strchr((*stack)->node->argv[cmd_i + i], '='))
			{
				internal_error(INT_ERR_4, "env",
					(*stack)->node->argv[cmd_i + i]);
				exit_status = 127;
			}
			i++;
		}
	}
	return (exit_status);
}
