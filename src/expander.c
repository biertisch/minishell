/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 22:09:38 by beatde-a         ###   ########.fr       */
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
	node->argv_info = ft_calloc(get_argc(node->argv), sizeof(t_metadata));
	validate_malloc(data, node->argv_info, NULL);
	i = 0;
	while (node->argv[i])
	{
		remove_quotes(data, &node->argv[i], &node->argv_info[i]);
		expand_dollar(data, &node->argv[i], &node->argv_info[i]);
		expand_tilde(data, &node->argv[i], &node->argv_info[i]);
		i++;
	}
	node->argv = build_expanded_argv(data, node->argv, &node->argv_info);
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
	if (!redir || !redir->file)
		return (0);
	remove_quotes(data, &redir->file, &redir->info);
	if (expand_dollar_redir(data, redir))
		return (-1);
	expand_tilde(data, &redir->file, &redir->info);
	if (expand_wildcard_redir(data, redir))
		return (-1);
	return (0);
}

int	expand_dollar(t_data *data, char **arg, t_metadata *info)
{
	int	i;

	if (!arg || !*arg || !info)
		return (0);
	init_expand_metadata(data, info);
	i = 0;
	while ((*arg) && (*arg)[i])
	{
		if (is_dollar_expansion(*arg, info->quote_map, i))
		{
			*arg = expand_variable(data, *arg, info, i);
			i = -1;
		}
		i++;
	}
	return (0);
}
