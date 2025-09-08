/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:21 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/08 16:53:59 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//assumes value is delimitated by blank space or operators
//groups text within quotes
//checks for unsupported characters and unclosed quotes
static int	get_token_value(t_data *data, char *input, char **value, int *index)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input[i])
	{
		if (is_unsupported_char(data, quote, input[i]))
			return (-1);
		if (!quote && (ft_isspace(input[i]) || is_operator(input + i)))
			break ;
		if (!quote && is_quote(input[i]))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
		return (syntax_error(data, ERR_1, NULL));
	if (i == 0 && is_operator(input + i))
		i = get_operator_len(input + i);
	*value = ft_substr(input, 0, i);
	validate_malloc(data, value, NULL);
	*index += ft_strlen(*value);
	return (0);
}

static t_token_type	get_token_type(char *input)
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

static void	add_token(t_data *data, t_token_type type, char *value)
{
	t_token	*new_node;

	new_node = create_lexer_node(type, value);
	validate_malloc(data, new_node, value);
	add_lexer_node(&data->lexer_list, new_node);
}

//converts user input into a list of tokens
int	lexer(t_data *data)
{
	t_token_type	type;
	char			*value;
	int				i;

	i = 0;
	while (data->input[i])
	{
		value = NULL;
		while (ft_isspace(data->input[i]))
			i++;
		if (!data->input[i])
			break ;
		type = get_token_type(data->input + i);
		if (get_token_value(data, data->input + i, &value, &i))
			return (-1);
		add_token(data, type, value);
	}
	return (0);
}
