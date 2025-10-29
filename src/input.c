/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:20:51 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/21 22:51:52 by beatde-a         ###   ########.fr       */
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

int	handle_signal_interruption(t_data *data, char *line, int cont)
{
	data->exit_status = 128 + g_sig_received;
	g_sig_received = 0;
	if (cont)
	{
		free_command_data(data);
		free(line);
		return (INCOMPLETE);
	}
	return (VALID);
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
	execute(data);
	return (VALID);
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
		if (g_sig_received)
			handle_signal_interruption(data, NULL, 0);
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
