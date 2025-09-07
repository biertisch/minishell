/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:49:39 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/04 12:49:39 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	parent(t_stack **stack, pid_t pid)
{
	t_stack **first_pipe;

	first_pipe = get_first_pipe(stack);
	if (first_pipe && *first_pipe)
		(*first_pipe)->child_pid[(*first_pipe)->child_count++] = pid;
	return (1);
}
