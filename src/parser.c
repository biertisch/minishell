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

//TODO: not sure if I need to handle other types besides AND_IF and OR_IF
/*PURPOSE: convert token type into AST node type*/
static t_node_type	get_node_type(t_token_type token_type)
{
	if (token_type == TOKEN_WORD)
		return (NODE_CMD);
	else if (token_type == TOKEN_PIPE)
		return (NODE_PIPE);
	else if (token_type == TOKEN_AND_IF)
		return (NODE_AND_IF);
	else if (token_type == TOKEN_OR_IF)
		return (NODE_OR_IF);
	else if (token_type == TOKEN_LPAREN || token_type == TOKEN_RPAREN)
		return (NODE_SUBSHELL);
}

//t_ast	*parse_command(t_data *data, t_token **token)

/*PURPOSE: parse pipe operator (|)
handles multiple pipe operators from left to right
calls parse_command() for left and right nodes
verifies if there is at least one command on each side of the pipe*/
t_ast	*parse_pipe(t_data *data, t_token **token)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(data, token);
	//if (!left)
		//syntactic error
	
	while ((*token) && (*token)->type == TOKEN_PIPE)
	{
		*token = (*token)->next;
		right = parse_command(data, token);
		//if (!right)
			//syntactic error
		left = create_parser_node(NODE_PIPE, left, right);
		//if (!left)
			//system error: failed malloc
	}
	return (left);
}

/*PURPOSE: parse logical operators (&& and ||)
handles multiple logical operators from left to right
calls parse_pipe() for left and right nodes
verifies if there is at least one command on each side of the operator*/
t_ast	*parse_and_or(t_data *data, t_token **token)
{
	t_ast			*left;
	t_ast			*right;
	t_token_type	type;

	left = parse_pipe(data, token);
	//if (!left)
		//syntactic error
	
	while ((*token) && ((*token)->type == TOKEN_AND_IF
		|| (*token)->type == TOKEN_OR_IF))
	{
		type = get_node_type((*token).type);
		*token = (*token)->next;
		right = parse_pipe(data, token);
		//if (!right)
			//syntactic error
		left = create_parser_node(type, left, right);
		//if (!left)
			//system error: failed malloc
	}
	return (left);
}

/*PURPOSE: process lexer list, examining relationship between tokens
& build abstract syntax tree (AST) based on operator precedence
from lowest precendece to highest: logical operators -> pipe -> commands*/
void	parser(t_data *data)
{
	t_token	*token;
	
	token = data->lexer_list;
	data->parser_list = parse_and_or(data, &token);
}
