/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:00:45 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/12 12:11:31 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//converts lexer token type into parser node type
t_node_type	get_node_type(t_token_type token_type)
{
	if (token_type == WORD)
		return (NODE_CMD);
	else if (token_type == PIPE)
		return (NODE_PIPE);
	else if (token_type == AND)
		return (NODE_AND);
	else if (token_type == OR)
		return (NODE_OR);
	else if (token_type == LPAREN || token_type == RPAREN)
		return (NODE_SUBSHELL);
	else
		return (-1);
}

int	is_command_token(t_token_type token_type)
{
	return (token_type == WORD || token_type == FD
		|| is_redir_token(token_type));
}

int	is_redir_token(t_token_type token_type)
{
	return (token_type == REDIR_IN || token_type == REDIR_OUT
		|| token_type == APPEND || token_type == HEREDOC);
}

int	empty_subshell(t_token **token, t_tree **root, t_tree *node, int res)
{
	if (*token && (*token)->type == RPAREN)
		*token = (*token)->next;
	*root = node;
	return (res);
}

int	parse_sub_tree(t_data *data, char *input, t_token *token, t_tree **root)
{
	t_token	*tmp;
	int		res;

	if (lexer(data, input, &token) || !token)
	{
		free(input);
		return (INVALID);
	}
	tmp = token;
	res = parse_and_or(data, &token, root);
	if (res == INVALID || !*root || scan_heredocs(data, *root) || token)
	{
		if (res != INVALID && token)
			syntax_error(data, SYN_ERR_5, token->value);
		free(input);
		free_lexer_list(&tmp);
		free_parser_tree(root);
		return (INVALID);
	}
	free(input);
	free_lexer_list(&tmp);
	return (res);
}
