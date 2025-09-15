/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:24 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/15 14:01:22 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//calls parse_and_or() for left node
//allows redirection (eg (ls)>file)
//checks for missing ')' and commands after subshell (eg (ls)ls)
static int	parse_subshell(t_data *data, t_token **token, t_tree **root)
{
	t_tree	*node;
	int		res;

	*token = (*token)->next;
	node = create_parser_node(NODE_SUBSHELL, NULL, NULL);
	validate_malloc(data, node, NULL);
	res = parse_and_or(data, token, &node->left);
	if (res)
		return (empty_subshell(token, node, res));
	if (!*token)
		return (prompt_continuation(data, ')'));
	if ((*token)->type != RPAREN)
		return (invalid_sequence(data, *token, node));
	*token = (*token)->next;
	if (*token && ((*token)->type == WORD || (*token)->type == LPAREN))
		return (invalid_sequence(data, *token, node));
	if (*token && (is_redir_token((*token)->type) || (*token)->type == FD)
		&& get_command_data(data, token, node))
		return (free_parser_tree(&node), INVALID);
	*root = node;
	return (VALID);
}

//calls parse_subshell() if it finds '('
//checks for missing commands
static int	parse_command(t_data *data, t_token **token, t_tree **root)
{
	t_tree	*node;

	if (!*token)
		return (prompt_continuation(data, 0));
	if (!is_command_token((*token)->type) && (*token)->type != LPAREN)
		return (syntax_error(data, ERR_1, (*token)->value));
	if ((*token)->type == LPAREN)
		return (parse_subshell(data, token, root));
	node = create_parser_node(NODE_CMD, NULL, NULL);
	validate_malloc(data, node, NULL);
	if (get_command_data(data, token, node))
		return (free_parser_node(&node), INVALID);
	if (node->argv && is_builtin(node->argv[0]))
		node->type = NODE_BUILTIN;
	*root = node;
	return (VALID);
}

//handles multiple pipe operators from left to right
//calls parse_command() for left and right nodes
//returns NULL if left or right node is empty
static int	parse_pipe(t_data *data, t_token **token, t_tree **root)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*tmp;
	int		res;

	res = parse_command(data, token, &left);
	if (res)
		return (res);
	while (*token && (*token)->type == PIPE)
	{
		*token = (*token)->next;
		res = parse_command(data, token, &right);
		if (res)
			return (free_parser_tree(&left), res);
		tmp = create_parser_node(NODE_PIPE, left, right);
		validate_malloc_tree(data, tmp, left, right);
		left = tmp;
	}
	*root = left;
	return (VALID);
}

//handles multiple logical operators from left to right
//calls parse_pipe() for left and right nodes
//returns NULL if left or right node is empty
int	parse_and_or(t_data *data, t_token **token, t_tree **root)
{
	t_tree		*left;
	t_tree		*right;
	t_tree		*tmp;
	t_node_type	type;
	int			res;

	res = parse_pipe(data, token, &left);
	if (res)
		return (res);
	while (*token && ((*token)->type == AND || (*token)->type == OR))
	{
		type = get_node_type((*token)->type);
		*token = (*token)->next;
		res = parse_pipe(data, token, &right);
		if (res)
			return (free_parser_tree(&left), res);
		tmp = create_parser_node(type, left, right);
		validate_malloc_tree(data, tmp, left, right);
		left = tmp;
	}
	*root = left;
	return (VALID);
}

//builds an abstract syntax tree (AST) based on operator precedence
//from lowest precendece to highest: logical operators -> pipe -> commands
//checks for stray ')'
int	parser(t_data *data)
{
	t_token	*token;
	int		res;

	token = data->lexer_list;
	res = parse_and_or(data, &token, &data->parser_tree);
	if (res)
		return (res);
	if (token && token->type == RPAREN) 
		return (syntax_error(data, ERR_1, token->value));
	if (token && token->type == LPAREN)
	{
		token = token->next;
		if (!token)
			return (syntax_error(data, ERR_1, "newline"));
		else
			return (syntax_error(data, ERR_1, token->value));		
	}
	return (VALID);
}
