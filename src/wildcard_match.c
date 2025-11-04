/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:39:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 22:07:14 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	filter_matches(t_list **head, char *pattern)
{
	t_list	*curr;
	t_list	*prev;
	t_list	*tmp;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (!match_wildcard((char *)curr->content, pattern))
		{
			tmp = curr;
			curr = curr->next;
			if (prev)
				prev->next = curr;
			else
				*head = curr;
			ft_lstdelone(tmp, free);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

int	match_wildcard(char *entry, char *pattern)
{
	int	i;
	int	j;
	int	star;

	i = 0;
	j = 0;
	star = -1;
	if (entry[0] == '.' && pattern[0] != '.')
		return (0);
	while (entry[i])
	{
		if (entry[i] == pattern[j])
			advance_both(&i, &j);
		else if (pattern[j] == '*')
			record_star(&j, &star);
		else if (!backtrack_on_mismatch(&i, &j, &star))
			return (0);
	}
	while (pattern[j] && pattern[j] == '*')
		j++;
	return (!pattern[j]);
}

void	advance_both(int *i, int *j)
{
	(*i)++;
	(*j)++;
}

void	record_star(int *j, int *star)
{
	*star = *j;
	(*j)++;
}

int	backtrack_on_mismatch(int *i, int *j, int *star)
{
	if (*star < 0)
		return (0);
	*j = *star + 1;
	(*i)++;
	return (1);
}
