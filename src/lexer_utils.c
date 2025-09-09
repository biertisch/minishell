/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 13:23:13 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/09 16:07:52 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_operator_len(char *s)
{
	if (!ft_strncmp(s, "<<", 2) || !ft_strncmp(s, ">>", 2)
		|| !ft_strncmp(s, "&&", 2) || !ft_strncmp(s, "||", 2))
		return (2);
	if (*s == '|' || *s == '<' || *s == '>' || *s == '(' || *s == ')')
		return (1);
	return (0);
}

int	is_operator(char *s)
{
	return (*s == '|' || *s == '<' || *s == '>' || *s == '(' || *s == ')'
		|| !ft_strncmp(s, "<<", 2) || !ft_strncmp(s, ">>", 2)
		|| !ft_strncmp(s, "&&", 2) || !ft_strncmp(s, "||", 2));
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

int	is_unsupported_char(t_data *data, char quote, char input)
{
	if (!quote && input == '\\')
		return (internal_error(data, ERR_1, NULL, "'\\'"));
	if (!quote && input == ';')
		return (internal_error(data, ERR_1, NULL, "';'"));
	return (0);
}
