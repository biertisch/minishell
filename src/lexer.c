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

//applies only to WORD and FD
//assumes value is delimitated by blank space or operators
//groups text within quotes as a single value
//checks for unsupported characters and unclosed quotes
static int	get_token_value(t_data *data, char *input, char **value, int *index)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\\' || input[i] == ';'))
			return (report_error("unsupported character", SYNTAX_ERR));
		if (!quote && (ft_isspace(input[i]) || is_operator(input + i)))
			break ;
		if (!quote && is_quote(input[i]))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
		return (report_error("missing quote", SYNTAX_ERR));
	*value = ft_substr(input, 0, i);
	validate_malloc(data, value, NULL);
	*index += ft_strlen(*value);
	return (0);
}

static int	update_index(t_token_type type)
{
	if (type == AND || type == OR
		|| type == HEREDOC || type == APPEND)
		return (2);
	else if (type == WORD || type == FD)
		return (0);
	else
		return (1);
}

static void	get_token_type(char *input, t_token_type *type, int *index)
{
	if (!ft_strncmp(input, "&&", 2))
		*type = AND;
	else if (!ft_strncmp(input, "||", 2))
		*type = OR;
	else if (!ft_strncmp(input, "<<", 2))
		*type = HEREDOC;
	else if (!ft_strncmp(input, ">>", 2))
		*type = APPEND;
	else if (*input == '|')
		*type = PIPE;
	else if (*input == '<')
		*type = REDIR_IN;
	else if (*input == '>')
		*type = REDIR_OUT;
	else if (*input == '(')
		*type = LPAREN;
	else if (*input == ')')
		*type = RPAREN;
	else if (is_fd(input))
		*type = FD;
	else
		*type = WORD;
	*index += update_index(*type);
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

	value = NULL;
	i = 0;
	while (data->input[i])
	{
		while (ft_isspace(data->input[i]))
			i++;
		if (!data->input[i])
			break ;
		get_token_type(data->input + i, &type, &i);
		value = NULL;
		if (type == WORD || type == FD)
		{
			if (get_token_value(data, data->input + i, &value, &i))
				return (-1);
			if (!value)
				continue ;
		}
		add_token(data, type, value);
	}
	return (0);
}
