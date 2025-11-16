/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 13:23:13 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/16 15:31:09 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char *s)
{
	return (s && (*s == '|' || *s == '<' || *s == '>' || *s == '(' || *s == ')'
			|| !ft_strncmp(s, "<<", 2) || !ft_strncmp(s, ">>", 2)
			|| !ft_strncmp(s, "&&", 2) || !ft_strncmp(s, "||", 2)));
}

int	get_operator_len(char *s)
{
	if (!s)
		return (0);
	if (!ft_strncmp(s, "<<", 2) || !ft_strncmp(s, ">>", 2)
		|| !ft_strncmp(s, "&&", 2) || !ft_strncmp(s, "||", 2))
		return (2);
	if (*s == '|' || *s == '<' || *s == '>' || *s == '(' || *s == ')')
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == '"')
		return (2);
	if (c == '\'')
		return (1);
	return (0);
}

//takes as FD what is only composed of digits
//and comes immediately before a redir operator (no spaces allowed)
int	is_fd(char *input)
{
	if (!input)
		return (0);
	while (*input && ft_isdigit(*input))
		input++;
	if (*input && (*input == '<' || *input == '>'
			|| !ft_strncmp(input, "<<", 2) || !ft_strncmp(input, ">>", 2)))
		return (1);
	return (0);
}

int	is_arithmetic_op(char *input)
{
	int	open;
	int	i;

	if (ft_strncmp(input, "((", 2))
		return (0);
	i = 2;
	open = 2;
	while (input[i])
	{
		if (input[i] == '(')
			open++;
		if (input[i] == ')')
		{
			if (open == 2 && !ft_strncmp(input + i, "))", 2))
				return (1);
			else
				open--;
		}
		if (open < 2)
			return (0);
		i++;
	}
	return (0);
}
