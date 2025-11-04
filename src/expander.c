/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 14:16:59 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	expand_argv(t_data *data, t_tree *node)
{
	int	i;

	if (!node || !node->argv)
		return (0);
	node->argv_info = ft_calloc(get_argc(node->argv), sizeof(t_arg_info));
	validate_malloc(data, node->argv_info, NULL);
	i = 0;
	while (node->argv[i])
	{
		remove_quotes(data, &node->argv[i], &node->argv_info[i]);
		expand_dollar(data, &node->argv[i], &node->argv_info[i]);
		expand_tilde(data, &node->argv[i], &node->argv_info[i]);
		i++;
	}
	node->argv = resize_argv(data, node->argv, &node->argv_info);
	expand_wildcard(data, node);
	return (0);
}

int	expand_redir(t_data *data, t_tree *node)
{
	t_redir	*trav;

	if (!node)
		return (0);
	trav = node->redir;
	while (trav)
	{
		if (trav->type != REDIR_IN && expand_single_redir(data, trav))
			return (-1);
		trav = trav->next;
	}
	return (0);
}

int	expand_single_redir(t_data *data, t_redir *redir)
{
	t_list	*entries;
	char	*tmp;

	if (!redir || !redir->file)
		return (0);
	remove_quotes(data, &redir->file, &redir->metadata);
	tmp = ft_strdup(redir->file);
	validate_malloc(data, tmp, NULL);
	expand_dollar(data, &redir->file, &redir->metadata);
	if (redir->metadata.key && !*(redir->metadata.value))
	{
		internal_error(data, ERR_2, NULL, tmp);
		return (free(tmp), 1);
	}
	free(tmp);
	expand_tilde(data, &redir->file, &redir->metadata);
	if (has_wildcard(redir->file, &redir->metadata))
	{
		if (expand_single_wildcard(data, redir->file, &entries))
			return (-1);
		if (entries)
		{
			if (entries->next)
			{
				ft_lstclear(&entries, free);
				return (internal_error(data, ERR_2, NULL, redir->file));
			}
			redir->file = update_redir_wildcard(data, redir->file, entries);
		}
	}
	return (0);
}
