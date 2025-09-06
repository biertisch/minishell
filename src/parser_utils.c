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

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

//classifies a substring as FD if it is only composed of digits
//and comes immediately before a redirection operator (no spaces allowed)
int	is_fd(char *input)
{
	while (*input && ft_isdigit(*input))
		input++;
	if (*input && (*input == '<' || *input == '>'
			|| !ft_strncmp(input, "<<", 2) || !ft_strncmp(input, ">>", 2)))
		return (1);
	return (0);
}

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

int	is_operator(char *s)
{
	return (*s == '|' || *s == '<' || *s == '>' || *s == '(' || *s == ')'
		|| !ft_strncmp(s, "<<", 2) || !ft_strncmp(s, ">>", 2)
		|| !ft_strncmp(s, "&&", 2));
}

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
