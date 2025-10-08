/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:20:51 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/08 15:49:19 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_eof(t_data *data)
{
	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	free_all(data);
	exit(data->exit_status);
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
	res = expand(data);
	if (res)
		return (res);
	execute(data);
	return (VALID);
}

int	prompt_continuation(t_data *data, char target)
{
	char	*line;

	rl_signal_event_hook = rl_sigint_continuation;
	while (1)
	{
		line = readline(CONTINUE_PROMPT);
		if (g_sig_received == SIGINT)
		{
			g_sig_received = 0;
			free_command_data(data);
			free(line);
			return (INCOMPLETE);
		}
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

void	prompt_input(t_data *data)
{
	int	status;

	rl_signal_event_hook = rl_sigint_main;
	while (1)
	{
		update_prompt(data);
		data->input = readline(data->prompt);
		if (!data->input)
			handle_eof(data);
		if (g_sig_received == SIGINT)
			g_sig_received = 0;
		if (*data->input)
		{
			status = process_input(data);
			if (status == INCOMPLETE && data->input)
			{
				process_input(data);
				rl_signal_event_hook = rl_sigint_main;
			}
			else if (status == INCOMPLETE_EOF)
				handle_eof(data);
		}
		free_command_data(data);
	}
}

void	read_input(t_data *data)
{
	int	status;

	while (1)
	{
		data->input = get_next_line(STDIN_FILENO);
		if (!data->input)
			handle_eof(data);
		if (*data->input)
		{
			status = process_input(data);
			if (status == INCOMPLETE && data->input)
			{
				process_input(data);
				rl_signal_event_hook = rl_sigint_main;
			}
			else if (status == INCOMPLETE_EOF)
				handle_eof(data);
		}
		free_command_data(data);
	}
}
