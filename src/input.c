/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:20:51 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/12 12:05:13 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_input(t_data *data)
{
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			update_prompt(data);
			data->input = readline(data->prompt);
		}
		else
			data->input = get_next_line(STDIN_FILENO);
		if (!data->input)
			eof_abort(data);
		if (g_sig == SIGINT)
			sigint_abort(data, NULL, 0);
		if (*data->input && process_input(data) == INCOMPLETE_EOF)
			eof_abort(data);
		free_command_data(data);
	}
	return (0);
}

int	process_input(t_data *data)
{
	int	res;

	add_history(data->input);
	res = lexer(data, data->input, &data->lexer_list);
	if (res || !data->lexer_list)
		return (res);
	res = parser(data, data->lexer_list, &data->parser_tree);
	if (res || !data->parser_tree)
		return (res);
	execute(data);
	return (VALID);
}

int	handle_incomplete_input(t_data *data)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		res;
	char	*input;

	if (pipe(pipe_fd))
		return (system_error(strerror(errno), "pipe"));
	pid = fork();
	if (pid < 0)
		return (system_error(strerror(errno), "fork"));
	else if (pid == 0)
		run_incomplete_child(data, 0, pipe_fd);
	res = run_incomplete_parent(data, pipe_fd, pid, &input);
	if (res)
		return (res);
	return (rebuild_tree(data, input));
}

int	read_continuation_input(t_data *data, char target, int out_fd)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline(CONTINUE_PROMPT);
		else
			line = get_next_line(STDIN_FILENO);
		if (g_sig == SIGINT)
			return (sigint_abort(data, line, 1));
		else if (!line)
			return (syntax_error(data, SYN_ERR_7, NULL));
		if (is_quote(target) || *line)
		{
			write(out_fd, " ", 1);
			write(out_fd, line, ft_strlen(line));
		}
		if ((target && ft_strchr(line, target)) || (!target && *line))
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (0);
}
