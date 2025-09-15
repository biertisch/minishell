/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:49:50 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/21 10:49:50 by beatde-a         ###   ########.fr       */
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

void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->file);
		free(redir);
		redir = tmp;
	}
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

//perhaps change to avoid recursion
void	free_parser_tree(t_tree **node)
{
	if (!node || !*node)
		return ;
	if ((*node)->left)
		free_parser_tree(&(*node)->left);
	if ((*node)->right)
		free_parser_tree(&(*node)->right);
	free_parser_node(node);
}

int	count_tree_nodes(t_tree *root)
{
	if (!root)
		return (0);
	return (1 + count_tree_nodes(root->left) + count_tree_nodes(root->right));
}
