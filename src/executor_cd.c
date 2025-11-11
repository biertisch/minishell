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
		write(STDERR_FILENO, "placeholder fucking shit\n", 25);
		(*stack)->exit_status = 1;
		return (0);
	}
	free(curr_pwd);
	curr_pwd = ft_strdup(getcwd(NULL, 0));
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
		execute_cd_option(data, stack, cmd_i, &chdir_res);
	if (chdir_res)
		(*stack)->exit_status = cd_fail(stack, (*stack)->node->argv[cmd_i + 1]);
	else
	{
		new_pwd = ft_strdup(getcwd(NULL, 0));
		validate_malloc_execute(data, stack, new_pwd, NULL);
		set_env_value(data->env_list, "OLDPWD", curr_pwd);
		set_env_value(data->env_list, "PWD", new_pwd);
	}
	if ((*stack)->exit_status != 0)
		free(curr_pwd);
	if (has_node_type_ancestor(*stack, NODE_PIPE))
		undo_duplicate_std(1);
	execute_builtin_check_for_pipe(data, stack);
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
		(*stack)->exit_status = cd_fail(stack, NULL);
	return (0);
}

int	cd_fail(t_stack **stack, char *dir)
{
	write(STDERR_FILENO, "minishell: cd: ", 15);
	write(STDERR_FILENO, dir, ft_strlen(dir));
	if (!dir)
	{
		if (has_builtin_flag((*stack)->node->argv))
			write(STDERR_FILENO, " placeholder flag\n", 18);
		else
			write(STDERR_FILENO, "too many arguments\n", 20);
	}
	else if (errno == ENOENT)
	{
		if (has_builtin_flag((*stack)->node->argv))
			write(STDERR_FILENO, " placeholder flag\n", 18);
		else
			write(STDERR_FILENO, ": No such file or directory\n", 28);
	}
	else if (errno == ENOTDIR)
		write(STDERR_FILENO, ": Not a directory\n", 18);
	else if (errno == EACCES)
		write(STDERR_FILENO, ": Permission denied\n", 20);
	return (1);
}
