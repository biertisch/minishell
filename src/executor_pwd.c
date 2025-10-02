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
	int	write_res;
	
	write_res = write((*stack)->out_fd, get_pwd(data), ft_strlen(get_pwd(data)));
	write_res = write((*stack)->out_fd, "\n", 1);
	(void)stack;
	exit(0);
	return (1);
}

char	*get_pwd(t_data *data)
{
	while ((data->env_list))
	{
		if (!ft_strcmp(data->env_list->key, "PWD"))
			return (data->env_list->value);
		data->env_list = data->env_list->next;
	}
	return (NULL);
}
