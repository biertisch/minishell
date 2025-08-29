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
static t_ast	*parse_subshell(t_data *data, t_token **token)
{
	t_ast	*node;
	char	**argv;
	t_redir	*redirs;

	*token = (*token)->next;
	node = create_parser_node(NODE_SUBSHELL, NULL, NULL, NULL);
	validate_malloc(data, node);
	node->left = parse_and_or(data, token);
	if (!node->left)
		return (NULL);
	if (!(*token) || (*token)->type != TOKEN_RPAREN)
		return (report_error("missing parenthesis", SYNTAX_ERR), NULL);
	*token = (*token)->next;
	if (*token && is_command_token((*token)->type))
	{
		if ((*token)->type == TOKEN_WORD)
			return (report_error("unexpected token", SYNTAX_ERR), NULL);
		argv = NULL;
		redirs = NULL;
		if (get_command_data(data, token, &argv, &redirs))
			return (NULL);
		node->cmd = create_command(argv, redirs);
		validate_malloc(data, node->cmd);
	}
	return (node);
}

//calls parse_subshell() if it finds '('
//checks for missing commands
static t_ast	*parse_command(t_data *data, t_token **token)
{
	t_ast	*node;
	t_cmd	*cmd;
	char	**argv;
	t_redir	*redirs;

	argv = NULL;
	redirs = NULL;
	if (!*token || (!is_command_token((*token)->type)
			&& (*token)->type != TOKEN_LPAREN))
		return (report_error("missing command", SYNTAX_ERR), NULL);
	if ((*token)->type == TOKEN_LPAREN)
		return (parse_subshell(data, token));
	if (get_command_data(data, token, &argv, &redirs))
		return (NULL);
	cmd = create_command(argv, redirs);
	validate_malloc(data, cmd);
	node = create_parser_node(NODE_CMD, cmd, NULL, NULL);
	validate_malloc(data, node);
	return (node);
}

//handles multiple pipe operators from left to right
//calls parse_command() for left and right nodes
//returns NULL if left or right node is empty
static t_ast	*parse_pipe(t_data *data, t_token **token)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(data, token);
	if (!left)
		return (NULL);
	while (*token && (*token)->type == TOKEN_PIPE)
	{
		*token = (*token)->next;
		right = parse_command(data, token);
		if (!right)
			return (NULL);
		left = create_parser_node(NODE_PIPE, NULL, left, right);
		validate_malloc(data, left);
	}
	return (left);
}

//handles multiple logical operators from left to right
//calls parse_pipe() for left and right nodes
//returns NULL if left or right node is empty
t_ast	*parse_and_or(t_data *data, t_token **token)
{
	t_ast		*left;
	t_ast		*right;
	t_node_type	type;

	left = parse_pipe(data, token);
	if (!left)
		return (NULL);
	while (*token && is_logical_token((*token)->type))
	{
		type = get_node_type((*token)->type);
		*token = (*token)->next;
		right = parse_pipe(data, token);
		if (!right)
			return (NULL);
		left = create_parser_node(type, NULL, left, right);
		validate_malloc(data, left);
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
	data->parser_list = parse_and_or(data, &token);
	if (token && token->type == TOKEN_RPAREN)
		return (report_error("unexpected token", SYNTAX_ERR));
	if (!data->parser_list)
		return (-1);
	return (0);
}
