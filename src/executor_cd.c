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

	curr_pwd = get_pwd(data);
	chdir_res = chdir((*stack)->node->argv[1]);
	if (chdir_res)
		perror("chdir");
	set_env_value(data->env_list, "OLD_PWD", curr_pwd);
	set_env_value(data->env_list, "PWD", (*stack)->node->argv[1]);
	(void)data;
	if (has_subshell_ancestor(*stack))
		exit(0);
	return (0);
}
