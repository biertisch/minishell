/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:49:50 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/30 12:45:01 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_parser_tree(t_tree **root)
{
	if (!root || !*root)
		return ;
	free_parser_tree(&(*root)->left);
	free_parser_tree(&(*root)->right);
	free_parser_node(root);
}

int	push_left_until_cmd(t_data *data)
{
	if (!data->stack || !data->stack->node)
		return (0);
	while (data->stack->node->left)
	{
		if (data->stack->node->type == NODE_SUBSHELL)
			data->stack->phase = DONE;
		push_stack(&data->stack, data->stack->node->left, 0, 0, data);
	}
	return (0);
}

int	count_tree_nodes(t_tree *root)
{
	if (!root)
		return (0);
	return (1 + count_tree_nodes(root->left) + count_tree_nodes(root->right));
}
