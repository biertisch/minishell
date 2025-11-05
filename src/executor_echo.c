/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:30:36 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:10:38 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_echo(t_data *data, t_stack **stack)
{
	int	cmd_i;

	cmd_i = get_first_command(data, stack);
	if (!(*stack)->node->argv[cmd_i + 1])
	{
		write(STDOUT_FILENO, "\n", 1);
		free_stack(stack);
		free_all(data);
		exit(0);
	}
	else if (is_echo_option((*stack)->node->argv[cmd_i + 1]))
		execute_echo_option(data, stack, cmd_i);
	else
		execute_echo_no_option(data, stack, cmd_i);
	return (0);
}

int	is_echo_option(char *opt)
{
	int	i;

	i = 1;
	if (*opt == '-')
	{
		while (*(opt + i) == 'n')
			i++;
		if (!*(opt + i))
			return (1);
	}
	return (0);
}

int	execute_echo_option(t_data *data, t_stack **stack, int cmd_i)
{
	int	i;

	i = 2 + cmd_i;
	while (is_echo_option((*stack)->node->argv[i]))
		i++;
	while ((*stack)->node->argv[i])
	{
		write(STDOUT_FILENO, (*stack)->node->argv[i],
			ft_strlen((*stack)->node->argv[i]));
		if ((*stack)->node->argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	close_all_open_redir_ends(data);
	close_all_pipe_ends(stack);
	free_stack(stack);
	free_all(data);
	exit(0);
	return (0);
}

int	execute_echo_no_option(t_data *data, t_stack **stack, int cmd_i)
{
	int	i;

	i = 1 + cmd_i;
	while ((*stack)->node->argv[i])
	{
		write(STDOUT_FILENO, (*stack)->node->argv[i],
			ft_strlen((*stack)->node->argv[i]));
		if ((*stack)->node->argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	write(STDOUT_FILENO, "\n", 1);
	close_all_open_redir_ends(data);
	close_all_pipe_ends(stack);
	free_stack(stack);
	free_all(data);
	exit(0);
	return (1);
}
