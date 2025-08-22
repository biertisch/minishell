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

//move is_quote and is_operator to another file (eg utils.c)
static int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

static int	is_operator(char c)
{
	return (c == '|' || c == '&' || c == '<' || c == '>'
		|| c == '(' || c == ')');
}

static void	update_quote_state(char c, char *quote)
{
	if (!*quote && is_quote(c))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

//add data to params to handle errors
static int	get_token_value(char *input, char **value)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input[i])
	{
		//if (!quote && (input[i] == '\\' || input[i] == ';'))
			//syntax error: unsupported character //CHECK WITH PEDRO -- perhaps add {}[]!:#%
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
    //if (quote)
		//syntax error: unclosed quote
	*value = ft_substr(input, 0, i);
	//if (!*value)
		//system error: memory allocation failed
	return (ft_strlen(*value));
}

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

void	lexer(t_data *data)
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
			i += get_token_value(data->input + i, &value);
			if (!value)
				continue ;
		}
		new_node = create_lexer_node(type, value);
		//if (!new_node)
			//system error: memory allocation failed
		add_lexer_node(&data->lexer_list, new_node);
	}
}
