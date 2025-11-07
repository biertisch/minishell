/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_cont.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:57:53 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/07 22:56:40 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (append_to_tree(data, input));
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

int	run_incomplete_parent(t_data *data, int *pipe_fd, pid_t pid, char **input)
{
	int	status;

	setup_signals_parent(data);
	close(pipe_fd[1]);
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		data->exit_status = WEXITSTATUS(status);
		if (data->exit_status)
		{
			close(pipe_fd[0]);
			setup_signals(data);
			if (data->exit_status == 2)
				return (free(*input), INCOMPLETE_EOF);
			if (data->exit_status == 130)
				return (free(*input), INVALID);
		}
	}
	*input = copy_continuation_input(data, pipe_fd[0]);
	close(pipe_fd[0]);
	setup_signals(data);
	return (VALID);
}

char	*copy_continuation_input(t_data *data, int in_fd)
{
	char	buffer[11];
	char	*new_input;
	ssize_t	read_bytes;

	new_input = NULL;
	ft_bzero(buffer, sizeof(buffer));
	read_bytes = read(in_fd, buffer, sizeof(buffer) - 1);
	while (read_bytes > 0)
	{
		buffer[read_bytes] = '\0';
		if (!new_input)
			new_input = ft_strdup(buffer);
		else
			new_input = ft_strdup_append(NULL, new_input, buffer);
		validate_malloc(data, data->input, NULL);
		read_bytes = read(in_fd, buffer, sizeof(buffer) - 1);
	}
	return (new_input);
}

int	append_to_tree(t_data *data, char *cont_input)
{
	t_token	*sub_list;
	t_tree	*sub_tree;

	sub_list = NULL;
	sub_tree = NULL;
	if (lexer(data, cont_input, &sub_list))
		return (INVALID);
	if (parser(data, sub_list, &sub_tree))
	{
		free(cont_input);
		free_lexer_list(&sub_list);
		return (INVALID);
	}
	if (!data->parser_tree)
		data->parser_tree = sub_tree;
	else
		data->parser_tree->right = sub_tree;
	free(cont_input);
	free_lexer_list(&sub_list);
	return (VALID);
}
