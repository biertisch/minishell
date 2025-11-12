/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:47:46 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 20:47:00 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_exit(t_data *data, t_stack **stack)
{
	int	exit_code;
	int	cmd_i;

	cmd_i = get_first_command(data, stack);
	duplicate_std();
	if (!has_node_type_ancestor(*stack, NODE_PIPE))
		handle_redirects(data, stack, NULL, (*stack)->node->redir);
	if ((*stack)->node->argv[cmd_i] && (*stack)->node->argv[cmd_i + 1])
		exit_code = ft_atoi((*stack)->node->argv[cmd_i + 1]) % 256;
	else if ((*stack)->node->argv[cmd_i] && !(*stack)->node->argv[cmd_i + 1])
		exit_code = data->exit_status;
	check_exit_input(data, stack, &exit_code, cmd_i);
	if (isatty(STDIN_FILENO) && !has_node_type_ancestor(*stack, NODE_PIPE) && !get_first_subshell(stack) && (!(*stack)->node->argv[1] || ((*stack)->node->argv[1] && !((*stack)->node->argv[2]))))
		write(STDERR_FILENO, "exit\n", 5);
	(*stack)->exit_status = exit_code;
	if (!(*stack)->node->argv[1] || !(*stack)->node->argv[2])
	{
		undo_duplicate_std(1);
		free_stack(stack);
		free_all(data);
		exit(exit_code);
	}
	return (1);
}

void	check_exit_input(t_data *data, t_stack **stack,
			int *exit_code, int cmd_i)
{
	int	i;

	if ((*stack)->node->argv[cmd_i + 1])
	{
		i = 0;
		if (*(*stack)->node->argv[cmd_i + 1] == '-'
			|| *(*stack)->node->argv[1] == '+')
			i++;
		while (*((*stack)->node->argv[cmd_i + 1] + i))
		{
			if (!ft_isdigit(*((*stack)->node->argv[cmd_i + 1] + i)))
			{
				write(STDERR_FILENO, "exit\n", 5);
				internal_error(INT_ERR_8, "exit", (*stack)->node->argv[1]);
				undo_duplicate_std(1);
				free_stack(stack);
				free_all(data);
				exit(2);
			}
			i++;
		}
		if ((*stack)->node->argv[cmd_i + 2])
		{
			write(STDERR_FILENO, "exit\n", 5);
			internal_error(INT_ERR_3, "exit", NULL);
			*exit_code = 1;
			undo_duplicate_std(1);
		}
	}
}
