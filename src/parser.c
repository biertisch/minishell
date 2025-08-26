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

t_ast	*parse_subshell(t_data *data, t_token **token)
{
	t_ast	*node;
	
	*token = (*token)->next;
	node = create_parser_node(NODE_SUBSHELL, NULL, NULL, NULL);
	validate_malloc(data, node);
	node->left = parse_and_or(data, token);
	if (!node->left)
		return (report_error("error inside subshell", SYNTAX_ERR), NULL);
	if (!(*token) || (*token)->type != TOKEN_RPAREN)
		return (report_error("missing parenthesis", SYNTAX_ERR), NULL);
	*token = (*token)->next;
	return (node);
}

/*PURPOSE: parse word tokens into commands*/
t_ast	*parse_command(t_data *data, t_token **token)
{
	t_ast	*node;
	t_cmd	*cmd;
	char	**argv;
	t_redir	*redirs;
	
	if (!(*token))
		return (report_error("missing command", SYNTAX_ERR), NULL);
	if ((*token)->type != TOKEN_WORD && (*token)->type != TOKEN_LPAREN
		&& !is_redir((*token)->type))
		return (report_error("unexpected token", SYNTAX_ERR), NULL);
	if ((*token)->type == TOKEN_LPAREN)
		return (parse_subshell(data, token));
	argv = get_command_argv(data, token);
	redirs = get_command_redirs(data, token);
	if (!argv && !redirs)
		return (report_error("empty command", SYNTAX_ERR), NULL);
	cmd = create_command(argv, redirs);
	validate_malloc(data, cmd);
	node = create_parser_node(NODE_CMD, cmd, NULL, NULL);
	validate_malloc(data, node);
	return (node);
}

/*PURPOSE: parse pipe operator (|)
handles multiple pipe operators from left to right
calls parse_command() for left and right nodes
verifies if there is at least one command on each side of the pipe*/
t_ast	*parse_pipe(t_data *data, t_token **token)
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

/*PURPOSE: parse logical operators (&& and ||)
handles multiple logical operators from left to right
calls parse_pipe() for left and right nodes
verifies if there is at least one command on each side of the operator*/
t_ast	*parse_and_or(t_data *data, t_token **token)
{
	t_ast		*left;
	t_ast		*right;
	t_node_type	type;

	left = parse_pipe(data, token);
	if (!left)
		return (NULL);
	while (*token && ((*token)->type == TOKEN_AND_IF
		|| (*token)->type == TOKEN_OR_IF))
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

/*PURPOSE: process lexer list, examining relationship between tokens
& build abstract syntax tree (AST) based on operator precedence
from lowest precendece to highest: logical operators -> pipe -> commands*/
int	parser(t_data *data)
{
	t_token	*token;
	
	token = data->lexer_list;
	data->parser_list = parse_and_or(data, &token);
	if (!data->parser_list)
		return (-1);
	return (0);
}
