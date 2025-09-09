/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:20:51 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/09 17:39:21 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*update_input(t_data *data, char *line, char target)
{
	char	*separator;
	char	*append_separator;
	char	*append_line;

	if (is_quote(target))
		separator = "\n";
	else
		separator = " ";
	append_separator = ft_strjoin(data->input, separator);
	validate_malloc(data, append_separator, line);
	append_line = ft_strjoin(append_separator, line);
	if (!append_line)
	{
		free(append_separator);
		validate_malloc(data, NULL, line);
	}
	free(append_separator);
	free_command_data(data);
	rl_replace_line(append_line, 0);
	return (append_line);
}

static int	incomplete_eof(t_data *data, char target)
{
	if (target && is_quote(target))
		syntax_error(data, ERR_7, &target);
	else
		syntax_error(data, ERR_8, NULL);
	return (INCOMPLETE_EOF);
}

int	prompt_continuation(t_data *data, char target)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			return (incomplete_eof(data, target));
		data->input = update_input(data, line, target);
		if ((target && ft_strchr(line, target)) || (!target && line[0]))
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (INCOMPLETE);
}

int	process_input(t_data *data)
{
	int	res;

	add_history(data->input);
	res = lexer(data);
	if (res)
		return (res);
	res = parser(data);
	if (res)
		return (res);
	res = expand(data, data->parser_tree);
	if (res)
		return (res);
	//execute //TODO PEDRO
	return (VALID);
}

void	prompt_input(t_data *data)
{
	int	status;

	while (1)
	{
		data->input = readline("minishell$ ");
		if (!data->input)
			handle_eof(data);
		if (data->input[0])
		{
			status = process_input(data);
			if (status == INCOMPLETE)
				process_input(data);
			else if (status == INCOMPLETE_EOF)
				handle_eof(data);
		}
		free_command_data(data);
	}
}
