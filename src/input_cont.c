/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_cont.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:57:53 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 22:40:04 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_incomplete_input(t_data *data, char target)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd))
	{
		print_pipe_err_mess();
		return (INVALID);
	}
	pid = fork();
	if (pid < 0)
	{
		print_fork_err_mess();
		return (INVALID);
	}
	else if (pid == 0)
		run_incomplete_child(data, target, pipe_fd);
	return (run_incomplete_parent(data, pipe_fd, pid));
}

int	run_incomplete_child(t_data *data, char target, int *pipe_fd)
{
	setup_signals_cont(data);
	close(pipe_fd[0]);
	data->exit_status = 0;
	prompt_input_cont(data, target, pipe_fd[1]);
	close(pipe_fd[1]);
	free_all(data);
	rl_clear_history();
	exit(data->exit_status);
}

int	write_to_pipe(char *line, char target, int fd)
{
	if (is_quote(target))
		write(fd, "\n", 1);
	else
		write(fd, " ", 1);
	write(fd, line, ft_strlen(line));
	return (0);
}

int	run_incomplete_parent(t_data *data, int *pipe_fd, pid_t pid)
{
	int	status;

	setup_signals_parent(data);
	close(pipe_fd[1]);
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		data->exit_status = WEXITSTATUS(status);
		if (data->exit_status == 2)
			return (close(pipe_fd[0]), INCOMPLETE_EOF);
		if (data->exit_status == 130)
			return (close(pipe_fd[0]), INVALID);
	}
	data->input = copy_continuation_input(data, pipe_fd);
	setup_signals(data);
	return (INCOMPLETE);
}

char	*copy_continuation_input(t_data *data, int *pipe_fd)
{
	char	buffer[11];
	char	*new_input;
	ssize_t	read_bytes;

	ft_bzero(buffer, sizeof(buffer));
	if (!data->input)
		new_input = ft_strdup("");
	else
		new_input = ft_strdup(data->input);
	validate_malloc(data, data->input, NULL);
	read_bytes = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	while (read_bytes > 0)
	{
		buffer[read_bytes] = '\0';
		new_input = ft_strdup_append(NULL, new_input, buffer);
		validate_malloc(data, data->input, NULL);
		read_bytes = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	}
	free_command_data(data);
	close(pipe_fd[0]);
	return (new_input);
}

// static char	*update_input(t_data *data, char *line, char target)
// {
// 	char	*separator;
// 	char	*append_separator;
// 	char	*append_line;

// 	if (is_quote(target))
// 		separator = "\n";
// 	else
// 		separator = " ";
// 	append_separator = ft_strjoin(data->input, separator);
// 	validate_malloc(data, append_separator, line);
// 	append_line = ft_strjoin(append_separator, line);
// 	if (!append_line)
// 	{
// 		free(append_separator);
// 		validate_malloc(data, NULL, line);
// 	}
// 	free(append_separator);
// 	free_command_data(data);
// 	rl_replace_line(append_line, 0);
// 	return (append_line);
// }
