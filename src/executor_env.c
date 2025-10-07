/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:39:33 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/01 18:08:54 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_env(t_data *data, t_stack **stack)
{
	int	i;

	i = 0;
	while ((data->env[i]))
	{
		write((*stack)->out_fd, data->env[i], ft_strlen(data->env[i]));
		write((*stack)->out_fd, "\n", 1);
		i++;
	}
	exit(0);
	return (1);
}
