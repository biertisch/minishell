/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:39:33 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/25 14:39:34 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_env(t_data *data, t_stack **stack)
{
	int	write_res;
	int	i;

	i = 0;
	printf("out_fd=%d\n", (*stack)->out_fd);
	while ((data->env[i]))
	{
		write_res = write((*stack)->out_fd, data->env[i], ft_strlen(data->env[i]));
		write_res = write((*stack)->out_fd, "\n", 1);
		i++;
	}
	exit(0);
	return (1);
}
