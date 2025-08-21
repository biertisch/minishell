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

t_ast	*create_parser_node(t_node_type type, char **argv, t_redir *redirs)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->left = NULL;
	new_node->right = NULL;
	if (type == NODE_CMD)
	{
		new_node->cmd = malloc(sizeof(t_cmd));
		if (!new_node->cmd)
		{
			free(new_node);
			return (NULL);
		}
		new_node->cmd->argv = argv;
		new_node->cmd->redirs = redirs;
	}
	else
		new_node->cmd = NULL;
	return (new_node);
}

void	free_parser_node(t_ast **node)
{
	if (!node || !*node)
		return ;
	if ((*node)->type == NODE_CMD && (*node)->cmd)
	{
		free_str_array(&(*node)->cmd->argv);
		//free_redirs
		free((*node)->cmd);
	}
	free(*node);
	*node = NULL;
}

//free_ast
