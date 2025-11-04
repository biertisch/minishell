/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 11:53:30 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/04 11:53:32 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*duplicate_std(void)
{
	static int	dups[4];

	if (dups[1] == 0)
	{
		dups[0] = dup(STDIN_FILENO);
		if (dups[0] < 0)
			return (dups);
		dups[1] = 1;
	}
	if (dups[3] == 0)
	{
		dups[2] = dup(STDOUT_FILENO);
		if (dups[2] < 0)
			return (dups);
		dups[3] = 1;
	}
	return (dups);
}

void	undo_duplicate_std(void)
{
	int	*dups;

	dups = duplicate_std();
	if (dups[1] != 0)
	{
		dup2(dups[0], STDIN_FILENO);
		close(dups[0]);
		dups[1] = 0;
	}
	if (dups[3] != 0)
	{
		dup2(dups[2], STDOUT_FILENO);
		close(dups[2]);
		dups[3] = 0;
	}
}
