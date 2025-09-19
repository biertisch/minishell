/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:49:50 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/18 11:52:38 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_tree	*create_parser_node(t_node_type type, t_tree *left, t_tree *right)
{
	t_tree	*new_node;

	new_node = malloc(sizeof(t_tree));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->left = left;
	new_node->right = right;
	new_node->argv = NULL;
	new_node->redir = NULL;
	return (new_node);
}

void	free_parser_node(t_tree **node)
{
	if (!node || !*node)
		return ;
	free_string_array(&(*node)->argv);
	free_redir((*node)->redir);
	free(*node);
	*node = NULL;
}

static int	clean_left_branch(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (-1); //issue warning?
	if (push_left_until_cmd(data, NULL))
		return (-1);
	free_parser_node(&data->stack->node);
	pop(&data->stack);
	return (0);
}

static int	clean_right_branch(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (0);
	while (data->stack)
	{
		if (data->stack->phase == DONE)
		{
			free_parser_node(&data->stack->node);
			pop(&data->stack);
			continue ;
		}
		if (data->stack->node->right)
		{
			data->stack->phase = DONE;
			push_right_once(data);
			if (clean_left_branch(data))
				return (-1);
		}
	}
	return (0);
}

void	free_parser_tree(t_data *data, t_tree **root)
{
	if (!root || !*root)
		return ;
	push_stack(&data->stack, *root, 0, 0, data);
	if (clean_left_branch(data))
		return ;
	if (clean_right_branch(data))
		return ;
	free_stack(&data->stack);
	data->stack = NULL; //incorporate into free_stack
	*root = NULL;
}

int	count_tree_nodes(t_tree *root)
{
	if (!root)
		return (0);
	return (1 + count_tree_nodes(root->left) + count_tree_nodes(root->right));
}
