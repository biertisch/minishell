/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:24 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/07 15:13:04 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//builds an abstract syntax tree based on operator precedence
//checks for stray parentheses at the end
int	parser(t_data *data, t_token *token, t_tree **parser_tree)
{
	int	res;

	res = parse_and_or(data, &token, parser_tree);
	if (check_for_heredoc(data))
		return (setup_signals(data), INVALID);
	if (res == INVALID)
		return (res);
	if (res == INCOMPLETE_PAREN || res == INCOMPLETE)
		return (handle_incomplete_input(data, res));
	if (token && token->type == RPAREN)
		return (syntax_error(data, SYN_ERR_5, token->value));
	if (token && token->type == LPAREN)
	{
		token = token->next;
		if (!token)
			return (syntax_error(data, SYN_ERR_5, "newline"));
		else
			return (syntax_error(data, SYN_ERR_5, token->value));
	}
	return (VALID);
}

int	parse_and_or(t_data *data, t_token **token, t_tree **root)
{
	t_tree		*left;
	t_tree		*right;
	t_tree		*tmp;
	t_node_type	type;
	int			res;

	res = parse_pipe(data, token, &left);
	if (res)
		return (*root = left, res);
	while (*token && ((*token)->type == AND || (*token)->type == OR))
	{
		type = get_node_type((*token)->type);
		*token = (*token)->next;
		res = parse_pipe(data, token, &right);
		tmp = create_parser_node(type, left, right);
		validate_malloc_parser(data, tmp, left, right);
		left = tmp;
		if (res)
			return (*root = left, res);
	}
	*root = left;
	return (VALID);
}

int	parse_pipe(t_data *data, t_token **token, t_tree **root)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*tmp;
	int		res;

	res = parse_command(data, token, &left);
	if (res)
		return (*root = left, res);
	while (*token && (*token)->type == PIPE)
	{
		*token = (*token)->next;
		res = parse_command(data, token, &right);
		tmp = create_parser_node(NODE_PIPE, left, right);
		validate_malloc_parser(data, tmp, left, right);
		left = tmp;
		if (res)
			return (*root = left, res);
	}
	*root = left;
	return (VALID);
}

//calls parse_subshell() if it finds '('
//checks for missing commands (INCOMPLETE or INVALID)
int	parse_command(t_data *data, t_token **token, t_tree **root)
{
	t_tree	*node;

	if (!*token)
		return (*root = NULL, INCOMPLETE);
	if (!is_command_token((*token)->type) && (*token)->type != LPAREN)
		return (*root = NULL, syntax_error(data, SYN_ERR_5, (*token)->value));
	if ((*token)->type == LPAREN)
		return (parse_subshell(data, token, root));
	node = create_parser_node(NODE_CMD, NULL, NULL);
	validate_malloc(data, node, NULL);
	if (get_command_data(data, token, node))
		return (*root = node, INVALID);
	*root = node;
	return (VALID);
}

//calls parse_and_or() for left node & allows redirection
//checks for unclosed parenthesis and invalid sequences
int	parse_subshell(t_data *data, t_token **token, t_tree **root)
{
	t_tree	*node;
	int		res;

	*token = (*token)->next;
	if (!*token)
		// return (INCOMPLETE_PAREN);
		return (syntax_error(data, SYN_ERR_0, ")"));
	node = create_parser_node(NODE_SUBSHELL, NULL, NULL);
	validate_malloc(data, node, NULL);
	res = parse_and_or(data, token, &node->left);
	if (res)
		return (empty_subshell(token, root, node, res));
	if (!*token)
		// return (*root = node, INCOMPLETE_PAREN);
		return (*root = node, syntax_error(data, SYN_ERR_0, ")"));
	if ((*token)->type != RPAREN)
		return (invalid_sequence(data, *token, root, node));
	*token = (*token)->next;
	if (*token && ((*token)->type == WORD || (*token)->type == LPAREN))
		return (invalid_sequence(data, *token, root, node));
	if (*token && (is_redir_token((*token)->type) || (*token)->type == FD)
		&& get_command_data(data, token, node))
		return (*root = node, INVALID);
	*root = node;
	return (VALID);
}
