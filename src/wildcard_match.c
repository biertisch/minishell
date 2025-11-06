/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:39:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/06 20:41:59 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	sort_list(t_list *head)
{
	t_list	*curr;
	t_list	*next;
	t_list	*tmp;

	if (!head)
		return ;
	curr = head;
	while (curr)
	{
		next = curr->next;
		while (next)
		{
			if (ft_strcmp(curr->content, next->content) > 0)
			{
				tmp = curr->content;
				curr->content = next->content;
				next->content = tmp;
			}
			next = next->next;
		}
		curr = curr->next;
	}
}
