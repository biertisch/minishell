/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 11:00:45 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/28 11:00:45 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//counts all TOKEN_WORD as argv, except for
//the one immediately after a redirection operator
int	count_argv(t_token *token)
{
	int	count;

	count = 0;
	while (token && is_command_token(token->type))
	{
		if (is_redir_token(token->type) && token->next
			&& token->next->type == TOKEN_WORD)
			token = token->next;
		else if (token->type == TOKEN_WORD)
			count++;
		token = token->next;
	}
	return (count);
}

int	is_command_token(t_token_type token_type)
{
	return (token_type == TOKEN_WORD || token_type == TOKEN_FD
		|| is_redir_token(token_type));
}

int	is_logical_token(t_token_type token_type)
{
	return (token_type == TOKEN_AND_IF || token_type == TOKEN_OR_IF);
}

int	is_redir_token(t_token_type token_type)
{
	return (token_type == TOKEN_REDIR_IN || token_type == TOKEN_REDIR_OUT
		|| token_type == TOKEN_APPEND || token_type == TOKEN_HEREDOC);
}

//converts lexer token type into parser node type
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
