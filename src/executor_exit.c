/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:47:46 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/30 14:47:47 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute_exit(t_data *data, t_stack **stack)
{
	int	exit_code;

	if ((*stack)->node->argv[1] && !(*stack)->node->argv[2])
		exit_code = ft_atoi((*stack)->node->argv[1]) % 256;
	else if ((*stack)->node->argv[1] && (*stack)->node->argv[2])
		exit_code = 1;
	else
		exit_code = data->exit_status;
	if (!get_first_subshell(stack))
		write(STDOUT_FILENO, "exit\n", 5);
	check_exit_input(stack, &exit_code);
	(*stack)->exit_status = exit_code;
	if (!(*stack)->node->argv[1] || !(*stack)->node->argv[2])
	{
		free_stack(stack);
		free_all(data);
		exit(exit_code);
	}
	return (1);
}

void	check_exit_input(t_stack **stack, int *exit_code)
{
	int	i;

	if ((*stack)->node->argv[1])
	{
		i = 0;
		while (*((*stack)->node->argv[1] + i))
		{
			if (!ft_isdigit(*((*stack)->node->argv[1] + i)))
			{
				write(STDERR_FILENO, "minishell : exit: ", 18);
				write(STDERR_FILENO, (*stack)->node->argv[1], ft_strlen((*stack)->node->argv[1]));
				write(STDERR_FILENO, ": numeric argument required\n", 28);
				*exit_code = 2;
				return ;
			}
			i++;
		}
		if ((*stack)->node->argv[2])
			write(STDERR_FILENO, "minishell : exit: too many arguments\n", 37);
		if ((*stack)->node->argv[2])
			*exit_code = 1;
	}
}
