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

static int	update_index(t_token_type type, char *value)
{
	if (type == TOKEN_AND_IF || type == TOKEN_OR_IF
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND)
		return (2);
	else if (type == TOKEN_WORD)
		return (ft_strlen(value));
	else
		return (1);
}

static int	is_operator(char c)
{
	return (c == '|' || c == '&' || c == '<' || c == '>'
		|| c == '(' || c == ')');
}

static char	*find_token_value(char *input) //add t_data *data to params
{
	char	*res;
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input[i])
	{
		if (!quote && (ft_is_space(input[i]) || is_operator(input[i])))
			break ;
		if (!quote && (input[i] == '"' || input[i] == '\''))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		i++;
	}
    //if (quote)
		//error
	res = ft_substr(input, 0, i);
	//if (!res)
		//error
	return (res);
}

static t_token_type	find_token_type(char *input)
{
	if (ft_strncmp(input, "&&", 2) == 0)
		return (TOKEN_AND_IF);
	else if (ft_strncmp(input, "||", 2) == 0)
		return (TOKEN_OR_IF);
	else if (ft_strncmp(input, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	else if (ft_strncmp(input, ">>", 2) == 0)
		return (TOKEN_APPEND);
	else if (*input == '|')
		return (TOKEN_PIPE);
	else if (*input == '<')
		return (TOKEN_REDIR_IN);
	else if (*input == '>')
		return (TOKEN_REDIR_OUT);
	else if (*input == '(')
		return (TOKEN_LPAREN);
	else if (*input == ')')
		return (TOKEN_RPAREN);
	else
		return (TOKEN_WORD);
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
		while (ft_is_space(data->input[i]))
			i++;
		if (!data->input[i])
			break ;
		type = find_token_type(data->input + i);
		value = NULL;
		if (type == TOKEN_WORD)
			value = find_token_value(data->input + i);
		i += update_index(type, value);
		new_node = create_lexer_node(type, value);
		//if (!new_node)
			//error
		add_lexer_node(&data->lexer_list, new_node);
	}
}
