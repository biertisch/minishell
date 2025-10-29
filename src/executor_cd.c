/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:14:33 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/28 12:14:35 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_cd(t_data *data, t_stack **stack)
{
	int		chdir_res;
	char	*curr_pwd;
	char	*new_pwd;

	(*stack)->exit_status = 0;
	chdir_res = 0;
	curr_pwd = ft_strdup(getcwd(NULL, 0));
	if (!(*stack)->node->argv[1])
		chdir_res = chdir(get_env_value(data->env_list, "HOME"));
	else if (!(*stack)->node->argv[2])
		chdir_res = chdir((*stack)->node->argv[1]);
	else
		(*stack)->exit_status = cd_fail(NULL);
	if (chdir_res)
		(*stack)->exit_status = cd_fail((*stack)->node->argv[1]);
	else
	{
		new_pwd = ft_strdup(getcwd(NULL, 0));
		set_env_value(data->env_list, "OLDPWD", curr_pwd);
		set_env_value(data->env_list, "PWD", new_pwd);
	}
	if (has_node_type_ancestor(*stack, NODE_SUBSHELL))
		exit((*stack)->exit_status);
	return (0);
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
		write(STDERR_FILENO, "too many arguments\n", 21);
	return (1);
}
