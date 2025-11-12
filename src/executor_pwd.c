/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:20:02 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:11:23 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pwd(t_data *data, t_stack **stack)
{
	int		exit_status;
	int		cmd_i;
	char	*pwd;

	cmd_i = get_first_command(data, stack);
	exit_status = 0;
	if ((*stack)->node->argv[cmd_i + 1]
		&& *(*stack)->node->argv[cmd_i + 1] == '-')
	{
		internal_error(INT_ERR_2, "pwd", (*stack)->node->argv[cmd_i + 1]);
		print_builtin_usage("pwd");
		exit_status = 2;
	}
	pwd = get_env_value(data->env_list, "PWD");
	if (!exit_status)
	{
		write((*stack)->out_fd, pwd, ft_strlen(pwd));
		write((*stack)->out_fd, "\n", 1);
	}
	executor_cleanup(data, stack, NULL);
	exit(exit_status);
	return (1);
}
