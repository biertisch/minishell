/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:49:50 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/21 10:49:50 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//GENERAL PURPOSE: helper functions to handle binary AST for lexer

t_ast	*create_parser_node(t_node_type type, t_cmd *cmd, t_ast *left,
	t_ast *right)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->left = left;
	new_node->right = right;
	new_node->cmd = cmd;
	return (new_node);
}

static void	free_redirs(t_redir *redir)
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

void	free_parser_node(t_ast **node)
{
	if (!node || !*node)
		return ;
	if ((*node)->type == NODE_CMD && (*node)->cmd)
	{
		free_str_array(&(*node)->cmd->argv);
		free_redirs((*node)->cmd->redirs);
		free((*node)->cmd);
	}
	free(*node);
	*node = NULL;
}

void	free_parser_list(t_ast **node)
{
	if (!node || !*node)
		return ;
	if ((*node)->left)
		free_parser_list(&(*node)->left);
	if ((*node)->right)
		free_parser_list(&(*node)->right);
	free_parser_node(node);
}
