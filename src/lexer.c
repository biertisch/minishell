/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:21 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/20 10:38:21 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*PURPOSE: check if char corresponds to opening or closing quote
helper to get_token_value()*/
static void	update_quote_state(char c, char *quote)
{
	if (!*quote && is_quote(c))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

/*PURPOSE: identify token value (applies only to TOKEN_WORD)
values are delimitated by blank space or operators
handles quotes: keeps text between quotes together and skips empty quotes
checks for errors: unsupported characters and unclosed quotes
helper to lexer()*/
static int	get_token_value(t_data *data, char *input, char **value)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\\' || input[i] == ';'))
			return (report_error("unsupported character", SYNTAX_ERR)); // CHECK perhaps add {}[]!:#%
		if (is_quote(input[i]) && is_quote(input[i + 1]))
		{
			*value = NULL;
			return (2);
		}
		if (!quote && (ft_isspace(input[i]) || is_operator(input[i])))
			break ;
		update_quote_state(input[i], &quote);
		i++;
	}
    if (quote)
		return (report_error("missing quote", SYNTAX_ERR));
	*value = ft_substr(input, 0, i);
	validate_malloc(data, value);
	return (ft_strlen(*value));
}

/*PURPOSE: calculate how many chars to skip in lexer loop
helper to get_token_type()*/
static int	update_index(t_token_type type)
{
	if (type == TOKEN_AND_IF || type == TOKEN_OR_IF
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND)
		return (2);
	else if (type == TOKEN_WORD)
		return (0);
	else
		return (1);
}

/*PURPOSE: identify token type
uses TOKEN_WORD as default for non-operators)
helper to lexer()*/
static int	get_token_type(char *input, t_token_type *type)
{
	if (ft_strncmp(input, "&&", 2) == 0)
		*type = TOKEN_AND_IF;
	else if (ft_strncmp(input, "||", 2) == 0)
		*type = TOKEN_OR_IF;
	else if (ft_strncmp(input, "<<", 2) == 0)
		*type = TOKEN_HEREDOC;
	else if (ft_strncmp(input, ">>", 2) == 0)
		*type = TOKEN_APPEND;
	else if (*input == '|')
		*type = TOKEN_PIPE;
	else if (*input == '<')
		*type = TOKEN_REDIR_IN;
	else if (*input == '>')
		*type = TOKEN_REDIR_OUT;
	else if (*input == '(')
		*type = TOKEN_LPAREN;
	else if (*input == ')')
		*type = TOKEN_RPAREN;
	else
		*type = TOKEN_WORD;
	return (update_index(*type));
}

/*PURPOSE: convert raw string into tokens in a singly linked list
prepares user input for parser*/
int	lexer(t_data *data)
{
	t_token			*new_node;
	t_token_type	type;
	char			*value;
	int				i;

	i = 0;
	while (data->input[i])
	{
		while (ft_isspace(data->input[i]))
			i++;
		if (!data->input[i])
			break ;
		i += get_token_type(data->input + i, &type);
		value = NULL;
		if (type == TOKEN_WORD)
		{
			i += get_token_value(data, data->input + i, &value);
			if (i == -1)
				return (-1);
			if (!value)
				continue ;
		}
		new_node = create_lexer_node(type, value);
		validate_malloc(data, new_node);
		add_lexer_node(&data->lexer_list, new_node);
	}
	return (0);
}
