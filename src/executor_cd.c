/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:14:33 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:10:35 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cd(t_data *data, t_stack **stack)
{
	int		chdir_res;
	char	*curr_pwd;
	char	*new_pwd;
	int		cmd_i;

	cmd_i = get_first_command(data, stack);
	(*stack)->exit_status = 0;
	chdir_res = 0;
	curr_pwd = ft_strdup(getcwd(NULL, 0));
	validate_malloc_execute(data, stack, curr_pwd, NULL);
	if (!(*stack)->node->argv[cmd_i + 1])
		chdir_res = chdir(get_env_value(data->env_list, "HOME"));
	else
		execute_cd_option(data, stack, cmd_i, &chdir_res);
	if (chdir_res)
		(*stack)->exit_status = cd_fail((*stack)->node->argv[cmd_i + 1]);
	else
	{
		new_pwd = ft_strdup(getcwd(NULL, 0));
		validate_malloc_execute(data, stack, new_pwd, NULL);
		set_env_value(data->env_list, "OLDPWD", curr_pwd);
		set_env_value(data->env_list, "PWD", new_pwd);
	}
	execute_cd_check_for_subshell(data, stack);
	return (0);
}

int	execute_cd_option(t_data *data, t_stack **stack, int cmd_i, int *chdir_res)
{
	if (!(*stack)->node->argv[cmd_i + 2])
	{
		if (!ft_strcmp((*stack)->node->argv[cmd_i + 1], "-"))
		{
			write(STDOUT_FILENO, get_env_value(data->env_list,
					"OLDPWD"), ft_strlen(get_env_value(data->env_list,
						"OLDPWD")));
			write(STDOUT_FILENO, "\n", 1);
			*chdir_res = chdir(get_env_value(data->env_list, "OLDPWD"));
		}
		else
			*chdir_res = chdir((*stack)->node->argv[cmd_i + 1]);
	}
	else
		(*stack)->exit_status = cd_fail(NULL);
	return (0);
}

void	execute_cd_check_for_subshell(t_data *data, t_stack **stack)
{
	int	exit_status;

	if (has_node_type_ancestor(*stack, NODE_SUBSHELL))
	{
		exit_status = (*stack)->exit_status;
		close_all_pipe_ends(stack);
		close_all_open_redir_ends(data);
		free_stack(stack);
		free_all(data);
		exit(exit_status);
	}
}

int	cd_fail(char *dir)
{
	write(STDERR_FILENO, "minishell: cd: ", 15);
	write(STDERR_FILENO, dir, ft_strlen(dir));
	if (errno == ENOENT)
		write(STDERR_FILENO, ": No such file or directory\n", 28);
	else if (errno == ENOTDIR)
		write(STDERR_FILENO, ": Not a directory\n", 18);
	else if (errno == EACCES)
		write(STDERR_FILENO, ": Permission denied\n", 20);
	else if (!dir)
		write(STDERR_FILENO, "too many arguments\n", 20);
	return (1);
}
