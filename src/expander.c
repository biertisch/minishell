/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/20 10:38:18 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	expand_redir(t_data *data, t_tree *node)
{
	t_redir	*trav;
	t_list	*entries;

	trav = node->redir;
	while (trav)
	{
		if (trav->file[0] != '\'')
			expand_dollar(data, &trav->file);
		if (!is_quote(trav->file[0]) && has_wildcard(trav->file))
		{
			if (expand_wildcard(data, trav->file, &entries))
				return (-1);
			if (entries && entries->next)
			{
				ft_lstclear(&entries, free);
				return (report_error("ambiguous redirect", INTERNAL_ERR));
			}
			if (entries)
				trav->file = update_redir(data, trav->file, entries);
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

	i = 0;
	while (node->argv[i])
	{
		if (node->argv[i][0] != '\'')
			expand_dollar(data, &node->argv[i]);
		if (!is_quote(node->argv[i][0]) && has_wildcard(node->argv[i]))
		{
			if (expand_wildcard(data, node->argv[i], &entries))
				return (-1);
			if (entries)
				node->argv = update_argv(data, node->argv, i, entries);
		}
		remove_quotes(data, &node->argv[i]);
		i++;
	}
	return (0);
}

//change to avoid recursion?
int	expand(t_data *data, t_tree *node)
{
	if (!node)
		return (-1);
	if ((node->type == NODE_CMD || node->type == NODE_BUILTIN) && node->argv)
		if (expand_argv(data, node))
			return (-1);
	if ((node->type == NODE_CMD || node->type == NODE_BUILTIN
			|| node->type == NODE_SUBSHELL) && node->redir)
		if (expand_redir(data, node))
			return (-1);
	expand(data, node->left);
	expand(data, node->right);
	return (0);
}
