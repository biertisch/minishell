/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:24 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/20 10:38:24 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//calls parse_and_or() for left node
//allows redirection (eg (ls)>file)
//checks for missing ')' and commands after subshell (eg (ls)ls)
static t_tree	*parse_subshell(t_data *data, t_token **token)
{
	t_tree	*node;

	*token = (*token)->next;
	node = create_parser_node(NODE_SUBSHELL, NULL, NULL);
	validate_malloc(data, node, NULL);
	node->left = parse_and_or(data, token);
	if (!node->left)
		return (free_parser_node(&node), NULL);
	if (!*token || (*token)->type != RPAREN)
	{
		report_error("missing parenthesis", SYNTAX_ERR);
		return (free_parser_tree(&node), NULL);
	}
	*token = (*token)->next;
	if (*token && ((*token)->type == WORD || (*token)->type == LPAREN))
	{
		report_error("unexpected token", SYNTAX_ERR);
		return (free_parser_tree(&node), NULL);
	}
	if (*token && (is_redir_token((*token)->type) || (*token)->type == FD)
		&& get_command_data(data, token, node))
		return (free_parser_tree(&node), NULL);
	return (node);
}

//calls parse_subshell() if it finds '('
//checks for missing commands
static t_tree	*parse_command(t_data *data, t_token **token)
{
	t_tree	*node;

	if (!*token || (!is_command_token((*token)->type)
			&& (*token)->type != LPAREN))
		return (report_error("missing command", SYNTAX_ERR), NULL);
	if ((*token)->type == LPAREN)
		return (parse_subshell(data, token));
	node = create_parser_node(NODE_CMD, NULL, NULL);
	validate_malloc(data, node, NULL);
	if (get_command_data(data, token, node))
		return (free_parser_node(&node), NULL);
<<<<<<< HEAD
=======
	if (is_builtin(node->argv[0]))
		node->type = NODE_BUILTIN;
>>>>>>> origin/dev
	return (node);
}

//handles multiple pipe operators from left to right
//calls parse_command() for left and right nodes
//returns NULL if left or right node is empty
static t_tree	*parse_pipe(t_data *data, t_token **token)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*tmp;

	left = parse_command(data, token);
	if (!left)
		return (NULL);
	while (*token && (*token)->type == PIPE)
	{
		*token = (*token)->next;
		right = parse_command(data, token);
		if (!right)
			return (free_parser_tree(&left), NULL);
		tmp = create_parser_node(NODE_PIPE, left, right);
		validate_malloc_tree(data, tmp, left, right);
		left = tmp;
	}
	return (left);
}

//handles multiple logical operators from left to right
//calls parse_pipe() for left and right nodes
//returns NULL if left or right node is empty
t_tree	*parse_and_or(t_data *data, t_token **token)
{
	t_tree		*left;
	t_tree		*right;
	t_tree		*tmp;
	t_node_type	type;

	left = parse_pipe(data, token);
	if (!left)
		return (NULL);
	while (*token && ((*token)->type == AND || (*token)->type == OR))
	{
		type = get_node_type((*token)->type);
		*token = (*token)->next;
		right = parse_pipe(data, token);
		if (!right)
			return (free_parser_tree(&left), NULL);
		tmp = create_parser_node(type, left, right);
		validate_malloc_tree(data, tmp, left, right);
		left = tmp;
	}
	return (left);
}

//builds an abstract syntax tree (AST) based on operator precedence
//from lowest precendece to highest: logical operators -> pipe -> commands
//checks for stray ')'
int	parser(t_data *data)
{
	t_token	*token;

	token = data->lexer_list;
	data->parser_tree = parse_and_or(data, &token);
	if (token && token->type == RPAREN)
		return (report_error("unexpected token", SYNTAX_ERR));
	if (!data->parser_tree)
		return (-1);
	return (0);
}
