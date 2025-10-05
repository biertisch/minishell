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
	if (!get_first_subshell(stack))
		write(1, "exit\n", 5);
	while (stack && *stack)
		pop(stack);
	free_all(data);
	exit(EXIT_SUCCESS);
	return (1);
}
