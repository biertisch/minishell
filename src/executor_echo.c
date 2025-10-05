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
		validate_write(data, stack, write((*stack)->out_fd, "\n", 1));
		free_stack(stack);
		free_all(data);
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
	int	i;

	i = 2;
	while ((*stack)->node->argv[i])
	{
		validate_write(data, stack, write((*stack)->out_fd, (*stack)->node->argv[i], ft_strlen((*stack)->node->argv[i])));
		if ((*stack)->node->argv[i + 1])
			validate_write(data, stack, write((*stack)->out_fd, " ", 1));
		i++;
		
	}
	free_stack(stack);
	free_all(data);
	exit(0);
	return (0);
}

int	execute_echo_no_option(t_data *data, t_stack **stack)
{
	int	i;

	i = 1;
	while ((*stack)->node->argv[i])
	{
		validate_write(data, stack, write((*stack)->out_fd, (*stack)->node->argv[i], ft_strlen((*stack)->node->argv[i])));
		if ((*stack)->node->argv[i + 1])
			validate_write(data, stack, write((*stack)->out_fd, " ", 1));
		i++;
	}
	validate_write(data, stack, write((*stack)->out_fd, "\n", 1));
	free_stack(stack);
	free_all(data);
	exit(0);
	return (1);
}

int	validate_write(t_data *data, t_stack **stack, int write_res)
{
	if (write_res == -1)
	{
		free_stack(stack);
		free_all(data);
		if (errno == EBADF || errno == ENOSPC || errno == EIO || errno == EROFS)
			exit(1);
		if (errno == EPIPE)
			exit(141);
	}
	return (write_res);
}
