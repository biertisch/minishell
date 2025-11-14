/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_continuation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:57:53 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/13 21:57:03 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_incomplete_child(t_data *data, char target, int *pipe_fd)
{
	setup_signals_cont(data);
	close(pipe_fd[0]);
	data->exit_status = 0;
	read_continuation_input(data, target, pipe_fd[1]);
	close(pipe_fd[1]);
	free_all(data);
	exit(data->exit_status);
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
	*input = receive_continuation_input(data, pipe_fd[0]);
	close(pipe_fd[0]);
	setup_signals(data);
	return (VALID);
}

char	*receive_continuation_input(t_data *data, int in_fd)
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

int	rebuild_tree(t_data *data, char *cont_input)
{
	t_token	*sub_list;
	t_tree	*sub_tree;
	int		res;

	sub_list = NULL;
	sub_tree = NULL;
	res = parse_sub_tree(data, cont_input, sub_list, &sub_tree);
	if (res == INVALID)
		return (INVALID);
	merge_sub_tree(data, sub_tree);
	if (res == INCOMPLETE)
		return (handle_incomplete_input(data));
	return (VALID);
}

int	merge_sub_tree(t_data *data, t_tree *sub_tree)
{
	t_tree	*far_left;
	t_tree	*far_left_parent;

	if (!data->parser_tree)
		data->parser_tree = sub_tree;
	else if (!sub_tree->left)
		data->parser_tree->right = sub_tree;
	else
	{
		far_left = sub_tree;
		while (far_left && far_left->left)
		{
			far_left_parent = far_left;
			far_left = far_left->left;
		}
		data->parser_tree->right = far_left;
		far_left_parent->left = data->parser_tree;
		data->parser_tree = sub_tree;
	}
	return (VALID);
}
