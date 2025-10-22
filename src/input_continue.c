/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_continue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 22:50:20 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/21 22:51:35 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	prompt_continuation(t_data *data, char target)
{
	char	*line;

	rl_signal_event_hook = rl_sigint_continuation;
	while (1)
	{
		line = readline(CONTINUE_PROMPT);
		if (g_sig_received)
			return (handle_signal_interruption(data, line, 1));
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
