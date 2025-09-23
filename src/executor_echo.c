/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:30:36 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/23 13:30:37 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_echo(t_data *data, t_stack **stack)
{
	if (!(*stack)->node->argv[1])
	{
		write((*stack)->out_fd, "\n", 1);
		exit(0);
	}
	else if (!ft_strcmp((*stack)->node->argv[1], "-n"))
		execute_echo_option(data, stack);
	else
		execute_echo_no_option(data, stack);
	(void)data;
	return (0);
}

int	execute_echo_option(t_data *data, t_stack **stack)
{
	if (!(*stack)->node->argv[2])
		exit(0);
	else
	{
		write((*stack)->out_fd, (*stack)->node->argv[2], ft_strlen((*stack)->node->argv[2]));
		exit(0);
	}
	(void)data;
	return (0);
}

int	execute_echo_no_option(t_data *data, t_stack **stack)
{
	write((*stack)->out_fd, (*stack)->node->argv[1], ft_strlen((*stack)->node->argv[1]));
	write((*stack)->out_fd, "\n", 1);
	exit(0);
	(void)data;
	return (1);
}
