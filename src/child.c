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
	check_for_errors(dup2(((*stack)->node->redir)->fd, STDIN_FILENO), data, *stack, "dup2");
	check_for_errors(dup2((*stack)->out_fd, STDOUT_FILENO), data, *stack, "dup2");
//	close(((*stack)->node->redir)->fd);
//	close((*stack)->pipe[0]);
//	close((*stack)->pipe[1]);
//	close((*stack)->out_fd);
	check_for_errors(execve((*stack)->node->argv[0], (*stack)->node->argv, data->env), data, *stack, "execve");
}

void	child_no_redir(t_data *data, t_stack **stack)
{
	check_for_errors(dup2((*stack)->out_fd, STDOUT_FILENO), data, *stack, "dup2");
	if ((*stack)->old_fd != -1)
		check_for_errors(dup2((*stack)->old_fd, STDIN_FILENO), data, *stack, "dup2");
	else
		check_for_errors(dup2((*stack)->in_fd, STDIN_FILENO), data, *stack, "dup2");
	check_for_errors(execve((*stack)->node->argv[0], (*stack)->node->argv, data->env), data, *stack, "execve");
}

void	child_redir_out(t_data *data, t_stack **stack)
{
	((*stack)->node->redir)->fd = open(((*stack)->node->redir)->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (((*stack)->node->redir)->fd < 0)
		check_for_errors(-1, data, *stack, "open");
	check_for_errors(dup2(((*stack)->node->redir)->fd, STDOUT_FILENO), data, *stack, "dup2");
	check_for_errors(dup2((*stack)->in_fd, STDIN_FILENO), data, *stack, "dup2");
	close(((*stack)->node->redir)->fd);
	close((*stack)->in_fd);
	check_for_errors(execve((*stack)->node->argv[0], (*stack)->node->argv, data->env), data, *stack, "execve");
}
