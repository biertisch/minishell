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
	int	write_res;

	if (!(*stack)->node->argv[1])
	{
		write_res = write((*stack)->out_fd, "\n", 1);
		if (write_res == -1)
			write_fail();
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
	int	write_res;

	if (!(*stack)->node->argv[2])
		exit(0);
	else
	{
		write_res =write((*stack)->out_fd, (*stack)->node->argv[2], ft_strlen((*stack)->node->argv[2]));
		if (write_res == -1)
			write_fail();
		exit(0);
	}
	(void)data;
	return (0);
}

int	execute_echo_no_option(t_data *data, t_stack **stack)
{
	int	write_res;

	write_res = write((*stack)->out_fd, (*stack)->node->argv[1], ft_strlen((*stack)->node->argv[1]));
	if (write_res == -1)
		write_fail();
	write((*stack)->out_fd, "\n", 1);
	if (write_res == -1)
		write_fail();
	exit(0);
	(void)data;
	return (1);
}

int	write_fail(void)
{
	if (errno == EBADF || errno == ENOSPC || errno == EIO || errno == EROFS)
		exit(1);
	if (errno == EPIPE)
		exit(141);
	return (1);
}
