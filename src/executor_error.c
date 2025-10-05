/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:46:03 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/02 12:46:05 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	validate_fork(t_data *data, t_stack **stack)
{
	int	i;

	i = 0;
	if (has_node_type_ancestor(*stack, NODE_PIPE))
	{
		while (has_node_type_ancestor(*stack, NODE_PIPE))
		{
			pop(stack);
			i++;
		}
		data->exit_status = 1;
	}
	else
		(*stack)->exit_status = 0;
	print_fork_err_mess();
	return (i);
}

void	print_fork_err_mess(void)
{
	if (errno == EAGAIN)
		write(2, "minishell: fork: Resource temporarily unavailable\n", 50);
	else if (errno == ENOMEM)
		write(2, "minishell: fork: Cannot allocate memory\n", 40);
	else
		write(2, "minishell: fork: Unknown error\n", 31);
}

int	validate_pipe(int pipe_res, t_stack **stack)
{
	if (pipe_res == 0)
		return (0);
	close_all_pipe_ends(stack);
	(*stack)->phase = DONE;
	(*stack)->exit_status = 1;
	print_pipe_err_mess();
	return (1);
}

void	print_pipe_err_mess(void)
{
	if (errno == EMFILE)
		write(2, "minishell: pipe: Too many open files\n", 37);
	else if (errno == ENFILE)
		write(2, "minishell: pipe: Too many open files in system\n", 47);
	else if (errno == ENOMEM)
		write(2, "minishell: pipe: Cannot allocate memory\n", 40);
	else
		write(2, "minishell: pipe: Unknown error\n", 31);
}
