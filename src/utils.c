/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:38:05 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/25 11:38:05 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_logical_op(t_token_type token_type)
{
	return (token_type == TOKEN_AND_IF || token_type == TOKEN_OR_IF);
}

int	is_redir(t_token_type token_type)
{
	return (token_type == TOKEN_REDIR_IN || token_type == TOKEN_REDIR_OUT
		|| token_type == TOKEN_APPEND || token_type == TOKEN_HEREDOC);
}

//PURPOSE: convert token type into AST node type
t_node_type	get_node_type(t_token_type token_type)
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
	else
		return (-1);
}

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

int	is_operator(char c)
{
	return (c == '|' || c == '&' || c == '<' || c == '>'
		|| c == '(' || c == ')');
}
