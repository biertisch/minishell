/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/30 18:54:47 by beatde-a         ###   ########.fr       */
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
				return (internal_error(data, ERR_2, NULL, trav->file));
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
		if (!is_quote(node->argv[i][0]))
			expand_tilde(data, &node->argv[i]);
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

static int	expand_left_branch(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (-1); //issue warning?
	if (push_left_until_cmd(data, expand_redir))
		return (-1);
	if (data->stack->node->argv)
		if (expand_argv(data, data->stack->node))
			return (-1);
	if (data->stack->node->redir)
		if (expand_redir(data, data->stack->node))
			return (-1);
	pop(&data->stack);
	return (0);
}

static int	expand_right_branch(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (0);
	while (data->stack)
	{
		if (data->stack->phase == DONE)
		{
			pop(&data->stack);
			continue ;
		}
		if (data->stack->node->right)
		{
			data->stack->phase = DONE;
			push_right_once(data);
			if (expand_left_branch(data))
				return (-1);
		}
	}
	return (0);
}

int	expand(t_data *data)
{
	push_stack(&data->stack, data->parser_tree, 0, 0, data);
	if (expand_left_branch(data))
		return (-1);
	if (expand_right_branch(data))
		return (-1);
	free_stack(&data->stack);
	return (0);
}
