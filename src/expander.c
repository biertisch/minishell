/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/02 15:26:12 by beatde-a         ###   ########.fr       */
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
	node->raw_argv = copy_string_array(node->argv);
	validate_malloc(data, node->raw_argv, NULL);
	i = 0;
	while (node->argv[i])
	{
		if (expand_single_arg(data, &node->argv[i], node->raw_argv[i]))
			return (-1);
		i++;
	}
	// if (resize_argv(data, &node->argv, node->raw_argv))
	// 	return (-1);
	return (0);
}

int	expand_single_arg(t_data *data, char **arg, char *raw_arg)
{
	while (has_dollar(*arg))
		if (expand_dollar(data, arg))
			return (-1);
	while (has_tilde(*arg))
		expand_tilde(data, arg);
	if (has_wildcard(*arg) && expand_wildcard(data, arg))
		return (-1);
	remove_quotes(data, arg, raw_arg);
	return (0);
}

	// for each arg in argv
	// - if it is not within single quotes, expand dollar until there is no dollar left
	// - if it is not within quotes, expand tilde
	// - if it is not within quotes, expand wildcard
	// - remove syntatic quotes (literal not expanded)
	// then resize array
	// - remove expanded empty args
	// - split expanded args with IFS (space, \n, \t)
	//
	// make argv and redir share the same logic, check afterwards if redir returns array with more than one string




// int	expand_argv(t_data *data, t_tree *node)
// {
// 	int		i;
// 	t_list	*entries;
// 	char	**old;

// 	i = 0;
// 	while (node->argv && node->argv[i])
// 	{
// 		old = node->argv;
// 		node->argv = expand_dollar(data, node->argv, i);
// 		if (node->argv != old)
// 		{
// 			i = 0;
// 			continue ;
// 		}
// 		expand_tilde(data, &node->argv[i]);
// 		if (!is_quote(node->argv[i][0]) && has_wildcard(node->argv[i])
// 			&& expand_wildcard(data, node->argv[i], &entries))
// 			return (-1);
// 		if (!is_quote(node->argv[i][0]) && has_wildcard(node->argv[i])
// 			&& entries)
// 			node->argv = update_argv_wildcard(data, node->argv, i, entries);
// 		remove_quotes(data, &node->argv[i]);
// 		i++;
// 	}
// 	return (0);
// }

int	expand_redir(t_data *data, t_tree *node)
{
	(void)data;
	(void)node;
	return (0);
	
	// t_redir	*trav;

	// trav = node->redir;
	// while (trav)
	// {
	// 	if (trav->type != REDIR_IN && expand_single_redir(data, trav))
	// 		return (-1);
	// 	trav = trav->next;
	// }
	// return (0);
}

int	expand_single_redir(t_data *data, t_redir *redir)
{
	(void)data;
	(void)redir;
	return (0);
	// t_list	*entries;

	// if (expand_dollar_redir(data, &redir->file))
	// 	return (-1);
	// expand_tilde(data, &redir->file);
	// if (redir->file && !is_quote(redir->file[0]) && has_wildcard(redir->file))
	// {
	// 	if (expand_wildcard(data, redir->file, &entries))
	// 		return (-1);
	// 	if (entries && entries->next)
	// 	{
	// 		ft_lstclear(&entries, free);
	// 		return (internal_error(data, ERR_2, NULL, redir->file));
	// 	}
	// 	if (entries)
	// 		redir->file = update_redir_wildcard(data, redir->file, entries);
	// }
	// remove_quotes(data, &redir->file);	// t_redir	*trav;

	// trav = node->redir;
	// while (trav)
	// {
	// 	if (trav->type != REDIR_IN && expand_single_redir(data, trav))
	// 		return (-1);
	// 	trav = trav->next;
	// }
	// return (0);
	// return (0);
}
