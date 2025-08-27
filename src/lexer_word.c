/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 12:16:55 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/27 12:16:55 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	check_empty_quote(char *input, int i, char **value, int *index)
{
	if (is_quote(input[i]) && is_quote(input[i + 1])
		&& input[i] == input[i + 1])
	{
		*value = NULL;
		*index += 2;
		return (1);
	}
	return (0);
}

//PURPOSE: check if char corresponds to opening or closing quote
//helper to get_token_value()
static void	update_quote_state(char c, char *quote)
{
	if (!*quote && is_quote(c))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

//PURPOSE: identify token value (applies only to TOKEN_WORD)
//values are delimitated by blank space or operators
//handles quotes: keeps text between quotes together and skips empty quotes
//checks for errors: unsupported characters and unclosed quotes
//helper to lexer()
int	get_token_value(t_data *data, char *input, char **value, int *index)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\\' || input[i] == ';'))
			return (report_error("unsupported character", SYNTAX_ERR)); // CHECK perhaps add {}[]!:#%
		if (check_empty_quote(input, i, value, index))
			return (0);
		if (!quote && (ft_isspace(input[i]) || is_operator(input[i])))
			break ;
		update_quote_state(input[i], &quote);
		i++;
	}
	if (quote)
		return (report_error("missing quote", SYNTAX_ERR));
	*value = ft_substr(input, 0, i);
	validate_malloc(data, value);
	*index += ft_strlen(*value);
	return (0);
}
