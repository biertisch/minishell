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

/*GENERAL PURPOSE: helper functions to handle binary AST for lexer*/

t_cmd	*create_command(char **argv, t_redir *redirs)
{
	t_cmd	*command;
	
	command = malloc(sizeof(t_cmd));
	if (!command)
		return (NULL);
	command->argv = argv;
	command->redirs = redirs;
	return (command);
}

t_ast	*create_parser_node(t_node_type type, t_cmd *cmd, t_ast *right,
	t_ast *left)
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
