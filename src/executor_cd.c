/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:14:33 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/10 14:36:53 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cd(t_data *data, t_stack **stack)
{
	char	*curr_pwd;

	duplicate_std();
	if (!has_node_type_ancestor(*stack, NODE_PIPE))
		handle_redirects(data, stack, NULL, (*stack)->node->redir);
	(*stack)->exit_status = 0;
	curr_pwd = getcwd(NULL, 0);
	if (!curr_pwd && !ft_strncmp((*stack)->node->argv[1], "..", 2))
	{
		free(curr_pwd);
		internal_error(INT_ERR_7, NULL, NULL);
		(*stack)->exit_status = 1;
		return (0);
	}
	free(curr_pwd);
	curr_pwd = getcwd(NULL, 0);
	validate_malloc_execute(data, stack, curr_pwd, NULL);
	return (execute_cd_continue(data, stack, curr_pwd));
}

int	execute_cd_continue(t_data *data, t_stack **stack, char *curr_pwd)
{
	char	*new_pwd;
	int		chdir_res;
	int		cmd_i;

	cmd_i = get_first_command(data, stack);
	if (!(*stack)->node->argv[cmd_i + 1])
		chdir_res = chdir(get_env_value(data->env_list, "HOME"));
	else
		chdir_res = execute_cd_option(data, stack, cmd_i);
	if (chdir_res)
		(*stack)->exit_status = cd_fail(data, stack, cmd_i);
	else
	{
		new_pwd = getcwd(NULL, 0);
		validate_malloc_execute(data, stack, new_pwd, NULL);
		set_env_value(data->env_list, "OLDPWD", curr_pwd);
		set_env_value(data->env_list, "PWD", new_pwd);
	}
	if (has_node_type_ancestor(*stack, NODE_PIPE))
		undo_duplicate_std(1);
	execute_builtin_check_for_pipe(data, stack);
	return (0);
}

int	execute_cd_option(t_data *data, t_stack **stack, int cmd_i)
{
	int	chdir_res;

	if (!(*stack)->node->argv[cmd_i + 2])
	{
		if (!ft_strcmp((*stack)->node->argv[cmd_i + 1], "-"))
		{
			write(STDOUT_FILENO, get_env_value(data->env_list,
					"OLDPWD"), ft_strlen(get_env_value(data->env_list,
						"OLDPWD")));
			write(STDOUT_FILENO, "\n", 1);
			chdir_res = chdir(get_env_value(data->env_list, "OLDPWD"));
		}
		else
			chdir_res = chdir((*stack)->node->argv[cmd_i + 1]);
	}
	else
		chdir_res = 1;
	return (chdir_res);
}

int	cd_fail(t_data *data, t_stack **stack, int cmd_i)
{
	if (has_builtin_flag((*stack)->node->argv))
	{
		internal_error(INT_ERR_2, "cd",
			(*stack)->node->argv[get_first_command(data, stack) + 1]);
		print_builtin_usage("cd");
		return (2);
	}
	if (!(*stack)->node->argv[cmd_i + 1])
		system_error(strerror(errno), "cd");
	else if ((*stack)->node->argv[cmd_i + 1] && (*stack)->node->argv[cmd_i + 2])
		internal_error(INT_ERR_3, "cd", NULL);
	else
		internal_error(INT_ERR_4, "cd", (*stack)->node->argv[cmd_i + 1]);
	return (1);
}
