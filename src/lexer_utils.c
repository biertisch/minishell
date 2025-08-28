/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 11:38:05 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/25 11:38:05 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	return (*s == '|' || *s == '<' || *s == '>'	|| *s == '(' || *s == ')'
		|| !ft_strncmp(s, "<<", 2) || !ft_strncmp(s, ">>", 2)
		|| !ft_strncmp(s, "&&", 2));
}
