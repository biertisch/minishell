/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 14:20:08 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/07 12:31:01 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_heredoc_child(t_data *data, t_redir *redir)
{
	setup_signals_heredoc(data);
	close(data->stack->pipe[0]);
	init_expand_metadata(data, &redir->info, redir->file);
	remove_quotes(data, &redir->file, &redir->info);
	heredoc(data, redir);
	close(data->stack->pipe[1]);
	free_all(data);
	rl_clear_history();
	exit(0);
}

int	heredoc(t_data *data, t_redir *redir)
{
	char	*line;

	while (1)
	{
		line = readline(CONTINUE_PROMPT);
		if (!line)
		{
			if (g_sig == SIGINT)
				return (heredoc_sigint_abort(data, line));
			else
				return (heredoc_eof_abort(data, redir->file));
		}
		else if (!ft_strcmp(line, redir->file))
		{
			free(line);
			break ;
		}
		write(data->stack->pipe[1], line, ft_strlen(line));
		write(data->stack->pipe[1], "\n", 1);
		free(line);
	}
	return (0);
}

int	run_heredoc_parent(t_data *data, t_redir *redir, pid_t pid)
{
	setup_signals_parent(data);
	copy_heredoc_input(data, redir);
	if (!has_quotes(redir->file))
		expand_dollar(data, &redir->heredoc_input, &redir->info);
	return (wait_for_heredoc(data, pid));
}

int	copy_heredoc_input(t_data *data, t_redir *redir)
{
	char	buffer[11];
	ssize_t	read_bytes;

	ft_bzero(buffer, sizeof(buffer));
	if (redir->heredoc_input)
		free(redir->heredoc_input);
	redir->heredoc_input = malloc(1);
	validate_malloc(data, redir->heredoc_input, NULL);
	redir->heredoc_input[0] = '\0';
	close(data->stack->pipe[1]);
	read_bytes = read(data->stack->pipe[0], buffer, sizeof(buffer) - 1);
	while (read_bytes > 0)
	{
		buffer[read_bytes] = '\0';
		redir->heredoc_input = ft_strdup_append(NULL, redir->heredoc_input,
				buffer);
		validate_malloc(data, redir->heredoc_input, NULL);
		read_bytes = read(data->stack->pipe[0], buffer, sizeof(buffer) - 1);
	}
	close(data->stack->pipe[0]);
	redir->info.total_len = ft_strlen(redir->heredoc_input);
	return (0);
}

int	wait_for_heredoc(t_data *data, pid_t pid)
{
	int		status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	return (data->exit_status == 130);
}
