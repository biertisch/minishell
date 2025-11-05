/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:21 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/05 12:33:39 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//converts user raw input into a list of tokens
int	lexer(t_data *data, char *input)
{
	t_token_type	type;
	char			*value;
	int				i;
	int				res;

	i = 0;
	while (input[i])
	{
		value = NULL;
		while (ft_isspace(data->input[i]))
			i++;
		if (!input[i])
			break ;
		res = get_token_value(data, input + i, &value, &i);
		if (res)
			return (res);
		type = get_token_type(value);
		add_token(data, &data->lexer_list, type, value);
	}
	return (VALID);
}

t_token_type	get_token_type(char *input)
{
	if (!ft_strncmp(input, "&&", 2))
		return (AND);
	else if (!ft_strncmp(input, "||", 2))
		return (OR);
	else if (!ft_strncmp(input, "<<", 2))
		return (HEREDOC);
	else if (!ft_strncmp(input, ">>", 2))
		return (APPEND);
	else if (*input == '|')
		return (PIPE);
	else if (*input == '<')
		return (REDIR_IN);
	else if (*input == '>')
		return (REDIR_OUT);
	else if (*input == '(')
		return (LPAREN);
	else if (*input == ')')
		return (RPAREN);
	else if (is_fd(input))
		return (FD);
	else
		return (WORD);
}

//takes as value what is delimitated by quotes, blank space or operators
//checks for unclosed quotes
int	get_token_value(t_data *data, char *input, char **value, int *index)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input[i])
	{
		if (!quote && check_unsupported_syntax(data, input + i))
			return (INVALID);
		if (!quote && (ft_isspace(input[i]) || is_operator(input + i)))
			break ;
		toggle_quote(input[i], &quote);
		i++;
	}
	if (quote) // perhaps refactor to helper & identify char in error msg
		return (syntax_error(data, ERR_8, NULL));
	if (i == 0 && is_operator(input + i))
		i = get_operator_len(input + i);
	*value = ft_substr(input, 0, i);
	validate_malloc(data, value, NULL);
	*index += ft_strlen(*value);
	return (VALID);
}

void	add_token(t_data *data, t_token **lexer_list, t_token_type type,
	char *value)
{
	t_token	*new_node;

	new_node = create_lexer_node(type, value);
	validate_malloc(data, new_node, value);
	add_lexer_node(lexer_list, new_node);
}

int	toggle_quote(char c, char *quote)
{
	if (!*quote && is_quote(c))
	{
		*quote = c;
		return (1);
	}
	else if (*quote && *quote == c)
	{
		*quote = 0;
		return (1);
	}
	return (0);
}
