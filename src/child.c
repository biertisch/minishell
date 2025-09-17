/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:49:31 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/04 12:49:32 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child(t_data *data, t_stack **stack)
{
	char	*full_path;

	env_list_to_array(data);
	full_path = correct_path(data, (*stack)->node->argv[0]);
	(*stack)->node->argv[0] = full_path;
	if ((*stack)->node->redir && ((*stack)->node->redir)->type == REDIR_IN)
		child_redir_in(data, stack);
	else if ((*stack)->node->redir && ((*stack)->node->redir)->type == REDIR_OUT)
		child_redir_out(data, stack);
	else
		child_no_redir(data, stack);
}

void	child_redir_in(t_data *data, t_stack **stack)
{
	((*stack)->node->redir)->fd = open((*stack)->node->redir->file, O_RDONLY);
	if (((*stack)->node->redir)->fd < 0)
		check_for_errors(-1, data, *stack, "open");
	printf("About to dup2 %d -> %d\t:35\n", ((*stack)->node->redir)->fd, STDIN_FILENO);
	check_for_errors(dup2(((*stack)->node->redir)->fd, STDIN_FILENO), data, *stack, "dup2");
	printf("About to dup2 %d -> %d\t:37\n", (*stack)->out_fd, STDOUT_FILENO);
	check_for_errors(dup2((*stack)->out_fd, STDOUT_FILENO), data, *stack, "dup2");
	close(((*stack)->node->redir)->fd);
	close((*stack)->out_fd);
	close((*get_first_pipe(stack))->pipe[0]);
	check_for_errors(execve((*stack)->node->argv[0], (*stack)->node->argv, data->env), data, *stack, "execve");
}

void	child_no_redir(t_data *data, t_stack **stack)
{
	printf("About to dup2 %d -> %d\t:47\n", (*stack)->in_fd, STDIN_FILENO);
	check_for_errors(dup2((*stack)->in_fd, STDIN_FILENO), data, *stack, "dup2");
	printf("About to dup2 %d -> %d\t:49\n", (*stack)->out_fd, STDOUT_FILENO);
	check_for_errors(dup2((*stack)->out_fd, STDOUT_FILENO), data, *stack, "dup2");
	close((*stack)->in_fd);
	close((*stack)->out_fd);
	check_for_errors(execve((*stack)->node->argv[0], (*stack)->node->argv, data->env), data, *stack, "execve");
}

void	child_redir_out(t_data *data, t_stack **stack)
{
	((*stack)->node->redir)->fd = open(((*stack)->node->redir)->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (((*stack)->node->redir)->fd < 0)
		check_for_errors(-1, data, *stack, "open");
	printf("About to dup2 %d -> %d\t:59\n", (*stack)->in_fd, STDIN_FILENO);
	check_for_errors(dup2((*stack)->in_fd, STDIN_FILENO), data, *stack, "dup2");
	printf("About to dup2 %d -> %d\t:61\n", ((*stack)->node->redir)->fd, STDOUT_FILENO);
	check_for_errors(dup2(((*stack)->node->redir)->fd, STDOUT_FILENO), data, *stack, "dup2");
	close(((*stack)->node->redir)->fd);
	close((*stack)->in_fd);
	close((*get_first_pipe(stack))->pipe[1]);
	check_for_errors(execve((*stack)->node->argv[0], (*stack)->node->argv, data->env), data, *stack, "execve");
}
