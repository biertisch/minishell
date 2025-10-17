/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:37:51 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/02 12:37:53 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute(t_data *data)
{
	t_stack	*stack;

	stack = create_stack(data);
	execute_stack(data, &stack);
	return (1);
}

int	execute_stack(t_data *data, t_stack **stack)
{
	int	i;
	int	tree_nodes_count;

	tree_nodes_count = count_tree_nodes(data->parser_tree);
	i = 0;
	while (i < tree_nodes_count)
	{
		if ((*stack)->type == NODE_PIPE)
			i += execute_pipe(data, stack);
		else if ((*stack)->type == NODE_CMD)
			i += execute_cmd(data, stack);
		else if	((*stack)->type == NODE_AND)
			i += execute_and(data, stack);
		else if ((*stack)->type == NODE_OR)
			i += execute_or(data, stack);
		else if ((*stack)->type == NODE_BUILTIN)
			i += execute_builtin(data, stack);
		else if ((*stack)->type == NODE_SUBSHELL)
			i += execute_subshell(data, stack);
	}
	pop(stack);
	return (0);
}

int	execute_cmd(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
		return (execute_cmd_entered(data, stack));
	if ((*stack)->phase == DONE)
		return (execute_cmd_done(&data, stack));
	return (0);
}

int	execute_cmd_entered(t_data *data, t_stack **stack)
{
	pid_t	pid;

	(*stack)->phase = DONE;
	if ((*stack)->node->redir && (*stack)->node->redir->type == HEREDOC && !(*stack)->node->argv)
		dummy_heredoc(stack);
	else
	{
		if ((*stack)->node->redir && (*stack)->node->redir->type == HEREDOC)
			if (validate_pipe(pipe((*stack)->pipe), stack))
				return (0);
		pid = fork();
		if (pid < 0)
			return (validate_fork(data, stack));
		else if (pid == 0)
			child(data, stack);
		else if ((*stack)->node->redir && (*stack)->node->redir->type == HEREDOC)
			parent_heredoc(stack, pid);
		else
			parent(stack, pid);
	}
	return (0);
}

int	execute_cmd_done(t_data **data, t_stack **stack)
{
	if ((*stack)->next)
		setup_next_to_top(data, stack);
	else if (stack_size(*stack) == 1 || !get_first_pipe(stack))
		(*data)->exit_status = (*stack)->exit_status;
	pop(stack);
	return (1);
}

int	dummy_heredoc(t_stack **stack)
{
	char	*line;
	char	*heredoc;

	heredoc = ft_strdup_append(NULL, ft_strdup((*stack)->node->redir->file), "\n");
	line = get_next_line(STDIN_FILENO);
	while (ft_strcmp(line, heredoc))
	{
		free(line);
		line = get_next_line(STDIN_FILENO);

	}
	free(line);
	free(heredoc);
	return (1);
}
