/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 12:20:02 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/01 18:09:33 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_pwd(t_data *data, t_stack **stack)
{
	char	*pwd;

	pwd = get_env_value(data->env_list, "PWD");
	if (pwd)
	{
		write((*stack)->out_fd, pwd, ft_strlen(pwd));
		write((*stack)->out_fd, "\n", 1);
		free_stack(stack);
		free_all(data);
		exit(0);
	}
	return (1);
}
