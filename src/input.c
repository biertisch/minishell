/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:20:51 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/05 14:57:46 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// non-interactive mode
int	read_input(t_data *data)
{
	int	status;

	if (isatty(STDIN_FILENO))
		return (prompt_input(data));
	while (1)
	{
		data->input = get_next_line(STDIN_FILENO);
		if (!data->input)
			eof_abort(data);
		if (*data->input)
		{
			status = process_input(data);
			if (status == INCOMPLETE && data->input)
				process_input(data);
			else if (status == INCOMPLETE_EOF)
				eof_abort(data);
		}
		free_command_data(data);
	}
	return (0);
}

// interactive mode
int	prompt_input(t_data *data)
{
	int	status;

	while (1)
	{
		update_prompt(data);
		data->input = readline(data->prompt);
		if (!data->input)
			eof_abort(data);
		if (g_sig)
			sigint_abort(data, NULL, 0);
		if (*data->input)
		{
			status = process_input(data);
			if (status == INCOMPLETE && data->input)
				process_input(data);
			else if (status == INCOMPLETE_EOF)
				eof_abort(data);
		}
		free_command_data(data);
	}
}

int	process_input(t_data *data)
{
	int	res;

	add_history(data->input);
	res = lexer(data, data->input);
	if (res || !data->lexer_list)
		return (res);
	print_lexer_list(data->lexer_list);
	res = parser(data, data->lexer_list);
	if (res || !data->parser_tree)
		return (res);
	print_parser_tree(data->parser_tree);
	execute(data);
	return (VALID);
}

int	prompt_input_cont(t_data *data, char target, int fd)
{
	char	*line;

	while (1)
	{
		line = readline(CONTINUE_PROMPT);
		if (g_sig == SIGINT)
			return (sigint_abort(data, line, 1));
		else if (!line)
			return (syntax_error(data, ERR_7, NULL));
		if (is_quote(target) || *line)
			write_to_pipe(line, target, fd);
		if ((target && ft_strchr(line, target)) || (!target && *line))
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (0);
}
