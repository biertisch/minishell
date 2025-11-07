/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_collect.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 14:20:08 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/07 22:52:20 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_heredoc_child(t_data *data, t_redir *redir, int *pipe_fd)
{
	setup_signals_heredoc(data);
	close(pipe_fd[0]);
	init_expand_metadata(data, &redir->info, redir->file);
	remove_quotes(data, &redir->file, &redir->info);
	collect_heredoc(data, redir, pipe_fd[1]);
	close(pipe_fd[1]);
	free_all(data);
	rl_clear_history();
	exit(0);
}

int	collect_heredoc(t_data *data, t_redir *redir, int out_fd)
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
		write(out_fd, line, ft_strlen(line));
		write(out_fd, "\n", 1);
		free(line);
	}
	return (0);
}

int	run_heredoc_parent(t_data *data, t_redir *redir, int *pipe_fd, pid_t pid)
{
	setup_signals_parent(data);
	close(pipe_fd[1]);
	copy_heredoc_input(data, redir, pipe_fd[0]);
	close(pipe_fd[0]);
	if (!has_quotes(redir->file))
		expand_dollar(data, &redir->heredoc_input, &redir->info);
	return (wait_heredoc(data, pid));
}

int	copy_heredoc_input(t_data *data, t_redir *redir, int in_fd)
{
	char	buffer[11];
	ssize_t	read_bytes;

	ft_bzero(buffer, sizeof(buffer));
	if (redir->heredoc_input)
		free(redir->heredoc_input);
	redir->heredoc_input = malloc(1);
	validate_malloc(data, redir->heredoc_input, NULL);
	redir->heredoc_input[0] = '\0';
	read_bytes = read(in_fd, buffer, sizeof(buffer) - 1);
	while (read_bytes > 0)
	{
		buffer[read_bytes] = '\0';
		redir->heredoc_input = ft_strdup_append(NULL, redir->heredoc_input,
				buffer);
		validate_malloc(data, redir->heredoc_input, NULL);
		read_bytes = read(in_fd, buffer, sizeof(buffer) - 1);
	}
	redir->info.total_len = ft_strlen(redir->heredoc_input);
	return (0);
}

int	wait_heredoc(t_data *data, pid_t pid)
{
	int		status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	setup_signals(data);
	return (data->exit_status == 130);
}
