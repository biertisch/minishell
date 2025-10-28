/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:20:51 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 13:02:10 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	process_input(t_data *data)
{
	int	res;

	add_history(data->input);
	res = lexer(data);
	if (res || !data->lexer_list)
		return (res);
	res = parser(data);
	if (res || !data->parser_tree)
		return (res);
	execute(data);
	return (VALID);
}

// interactive mode
void	prompt_input(t_data *data)
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

// non-interactive mode
void	read_input(t_data *data)
{
	int	status;

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
}

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

int	prompt_cont(t_data *data, char target)
{
	char	*line;

	while (1)
	{
		line = readline(CONTINUE_PROMPT);
		if (g_sig == SIGINT)
			return (sigint_abort(data, line, 1));
		if (!line)
			return (syntax_error(data, ERR_7, NULL));
		if (is_quote(target) || *line)
			data->input = update_input(data, line, target);
		if ((target && ft_strchr(line, target)) || (!target && *line))
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (INCOMPLETE);
}
