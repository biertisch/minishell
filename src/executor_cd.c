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

	curr_pwd = ft_strdup(getcwd(NULL, 0));
	chdir_res = chdir((*stack)->node->argv[1]);
	if (chdir_res)
		cd_fail((*stack)->node->argv[1]);
	new_pwd = ft_strdup(getcwd(NULL, 0));
	set_env_value(data->env_list, "OLDPWD", curr_pwd);
	set_env_value(data->env_list, "PWD", new_pwd);
	if (has_subshell_ancestor(*stack))
		exit(0);
	return (0);
}

int	cd_fail(char *dir)
{
	char	*err_mess;
	
	write(STDERR_FILENO, "minishell: ", 11);
	if (errno == ENOENT)
	{
		err_mess = malloc(34);
		err_mess = ft_strdup("cd: ");
		err_mess = ft_strdup_append(NULL, err_mess, dir);
		err_mess = ft_strdup_append(NULL, err_mess, ": No such file or directory\n");
		write(STDERR_FILENO, err_mess, 38);
		free(err_mess);
	}
	if (errno == ENOTDIR)
     	{
		err_mess = malloc(23);
		err_mess = ft_strdup("cd: ");
		err_mess = ft_strdup_append(NULL, err_mess, dir);
		err_mess = ft_strdup_append(NULL, err_mess, ": Not a directory\n");
		write(STDERR_FILENO, err_mess, 30);
		free(err_mess);
	}
	if (errno == EACCES)
	{
		err_mess = malloc(31);
		err_mess = ft_strdup("cd: ");
		err_mess = ft_strdup_append(NULL, err_mess, dir);
		err_mess = ft_strdup_append(NULL, err_mess, ": Permission denied\n");
		write(STDERR_FILENO, err_mess, 27);
		free(err_mess);
	}
	return (1);
}
