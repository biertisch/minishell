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
	t_stack	**first_pipe;

	first_pipe = get_first_pipe(stack);
	if (first_pipe && *first_pipe)
	{
		(*first_pipe)->child_pid[(*first_pipe)->child_count++] = pid;
		if ((*first_pipe)->old_fd != -1)
			if ((*first_pipe)->old_fd != (*first_pipe)->pipe[0])
				close((*first_pipe)->old_fd);
		(*first_pipe)->old_fd = (*first_pipe)->pipe[0];
		if ((*first_pipe)->phase == LAUNCH_RIGHT)
			parent_launch_right(first_pipe);
	}
	return (1);
}

int	parent_launch_right(t_stack **first_pipe)
{
	int	status;
	pid_t	res;

	close((*first_pipe)->pipe[1]);
	close((*first_pipe)->pipe[0]);
	if ((*first_pipe)->child_count == 1)
		res = waitpid((*first_pipe)->child_pid[0], &status, 0);
	else
		res = waitpid((*first_pipe)->child_pid[1], &status, 0);
	if (WIFEXITED(status))
		(*first_pipe)->exit_status = WEXITSTATUS(status);
	//for now it stays like this but i still need to check signals
	printf("exit_status = %d", (*first_pipe)->exit_status);
	return (1);
}
