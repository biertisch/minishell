/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 16:52:51 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_redir(t_data *data, t_tree *node)
{
	t_redir	*trav;
	t_list	*entries;

	trav = node->redir;
	while (trav)
	{
		if (expand_dollar_redir(data, &trav->file))
			return (-1);
		expand_tilde(data, &trav->file);
		if (trav->file && !is_quote(trav->file[0]) && has_wildcard(trav->file))
		{
			if (expand_wildcard(data, trav->file, &entries))
				return (-1);
			if (entries && entries->next)
			{
				ft_lstclear(&entries, free);
				return (internal_error(data, ERR_2, NULL, trav->file));
			}
			if (entries)
				trav->file = update_redir_wildcard(data, trav->file, entries);
		}
		remove_quotes(data, &trav->file);
		trav = trav->next;
	}
	return (0);
}

static int	expand_argv(t_data *data, t_tree *node)
{
	int		i;
	t_list	*entries;
	char	**old;

	i = 0;
	while (node->argv && node->argv[i])
	{
		old = node->argv;
		node->argv = expand_dollar(data, node->argv, i);
		if (node->argv != old)
		{
			i = 0;
			continue ;
		}
		expand_tilde(data, &node->argv[i]);
		if (!is_quote(node->argv[i][0]) && has_wildcard(node->argv[i])
			&& expand_wildcard(data, node->argv[i], &entries))
			return (-1);
		if (!is_quote(node->argv[i][0]) && has_wildcard(node->argv[i])
			&& entries)
			node->argv = update_argv_wildcard(data, node->argv, i, entries);
		remove_quotes(data, &node->argv[i]);
		i++;
	}
	return (0);
}

int	expand(t_data *data, t_tree *node)
{
	if (!node)
		return (-1);
	if (node->argv && expand_argv(data, node))
		return (-1);
	if (node->redir && expand_redir(data, node))
		return (-1);
	return (0);
}
