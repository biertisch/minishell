/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:39:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/03 15:03:12 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	backtrack_on_mismatch(int *i, int *j, int *star)
{
	if (*star < 0)
		return (0);
	*j = *star + 1;
	(*i)++;
	return (1);
}

static void	record_star(int *j, int *star)
{
	*star = *j;
	(*j)++;
}

static void	advance_both(int *i, int *j)
{
	(*i)++;
	(*j)++;
}

static int	is_hidden_file(char *entry, char *wildcard)
{
	return ((entry[0] == '.' && wildcard[0] != '.') || !ft_strcmp(entry, ".")
		|| !ft_strcmp(entry, ".."));
}

int	match_wildcard(char *entry, char *wildcard)
{
	int	i;
	int	j;
	int	star;

	i = 0;
	j = 0;
	star = -1;
	if (is_hidden_file(entry, wildcard))
		return (0);
	while (entry[i])
	{
		if (entry[i] == wildcard[j])
			advance_both(&i, &j);
		else if (wildcard[j] == '*')
			record_star(&j, &star);
		else if (!backtrack_on_mismatch(&i, &j, &star))
			return (0);
	}
	while (wildcard[j] && wildcard[j] == '*')
		j++;
	if (wildcard[j])
		return (0);
	return (1);
}
