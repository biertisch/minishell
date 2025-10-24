/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 12:37:51 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/21 22:43:55 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute(t_data *data)
{
	t_stack	*stack;

	check_for_heredoc(data);
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
		else if ((*stack)->type == NODE_SUBSHELL)
			i += execute_subshell(data, stack);
	}
	pop(stack);
	return (0);
}

int	execute_cmd(t_data *data, t_stack **stack)
{
	if ((*stack)->phase == ENTERED)
	{
		if (expand(data, (*stack)->node))
		{
			pop(stack);
			return (1); 
		}
	}
	if ((*stack)->node->argv && is_builtin((*stack)->node->argv[get_first_command(data, stack)]))
		return (execute_builtin(data, stack));
	else if ((*stack)->phase == ENTERED)
		return (execute_cmd_entered(data, stack));
	else if ((*stack)->phase == DONE)
		return (execute_cmd_done(&data, stack));
	return (0);
}

int	execute_cmd_entered(t_data *data, t_stack **stack)
{
	pid_t	pid;
	
	(*stack)->phase = DONE;
	if (!check_if_variable(data, stack))
	{
		if (get_last_heredoc((*stack)->node->redir) && !(*stack)->node->argv)
			return (0);
		//check this shit out
		if (get_last_heredoc((*stack)->node->redir))
			if (validate_pipe(pipe((*stack)->pipe), stack))
				return (0);
		pid = fork();
		if (pid < 0)
			return (validate_fork(data, stack));
		else if (pid == 0)
			child(data, stack);
		else if (get_last_heredoc((*stack)->node->redir))
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

int	dummy_heredoc(t_redir *redir)
{
	char	*line;
	char	*heredoc;

	if (redir->heredoc_input)
		free(redir->heredoc_input);
	redir->heredoc_input = malloc(1);
	redir->heredoc_input[0] = '\0';
	heredoc = ft_strdup_append(NULL, ft_strdup(redir->file), "\n");
	write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	while (ft_strcmp(line, heredoc))
	{
		redir->heredoc_input = ft_strdup_append(NULL, redir->heredoc_input, line);
		free(line);
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	free(heredoc);
	return (1);
}
