/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:49:39 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/04 12:49:39 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	parent(t_stack **stack, pid_t pid)
{
	t_stack	**first_pipe;
	t_stack **first_subshell;

	first_pipe = get_first_pipe(stack);
	first_subshell = get_first_subshell(stack);
	if (first_pipe && !first_subshell)
	{
		(*first_pipe)->child_pid[(*first_pipe)->child_count++] = pid;
	}
	else
		return (parent_single_command(stack, pid));
	return (1);
}

int	parent_single_command(t_stack **stack, pid_t pid)
{
	int		status;
	pid_t	res;

	res = waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*stack)->exit_status = WEXITSTATUS(status);
	return (1);
}

int	heredoc_loop(t_stack **stack, char *heredoc, char *res)
{
	char	*line;

	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line || !ft_strcmp(line, heredoc))
			break ;
		else
		{
			res = ft_strdup_append(NULL, res, line);
			free(line);
		}
	}
	write((*stack)->pipe[1], "\n", 1);
	write((*stack)->pipe[1], res, ft_strlen(res));
	if (line)
		free(line);
	free(res);
	free(heredoc);
	return (1);
}

int	parent_heredoc(t_stack **stack, pid_t pid)
{
	char	*line;
	char	*res;
	char	*heredoc;

	close((*stack)->pipe[0]);
	line = get_next_line(STDIN_FILENO);
	heredoc = ft_strdup_append(NULL, ft_strdup((*stack)->node->redir->file), "\n");
	if (ft_strcmp(line, heredoc))
	{
		res = ft_strdup(line);
		free(line);
		heredoc_loop(stack, heredoc, res);
	}
	else
		write((*stack)->pipe[1], "\n", 1);
	close((*stack)->pipe[1]);
	return (parent_single_command(stack, pid));
}
