/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 16:02:26 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 16:02:27 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_is_directory(t_data *data, t_stack **stack, int fd)
{
	if (fd != -1)
	{
		internal_error(INT_ERR_11,
			(*stack)->node->argv[get_first_command(data, stack)], NULL);
		close(fd);
		executor_cleanup(data, stack, NULL);
		exit(126);
	}
}

char	*run_curr_dir(t_data *data, t_stack **stack, char *cmd)
{
	char	*cmd_res;

	cmd_is_directory(data, stack, open(cmd, O_DIRECTORY));
	if (access(cmd, F_OK | X_OK) == -1)
		executor_child_errno(data, stack, NULL, NULL);
	cmd_res = ft_strdup(cmd);
	validate_malloc_execute(data, stack, cmd_res, NULL);
	return (cmd_res);
}

void	executor_child_errno(t_data *data, t_stack **stack,
		char *to_free, char **also_to_free)
{
	free(to_free);
	ft_splitfree(also_to_free);
	clean_execve_failure(data, stack);
}

void	check_for_variables(t_data *data, t_stack **stack)
{
	t_env	*new_node;
	int		i;
	int		j;

	i = 0;
	while (is_new_var((*stack)->node->argv[i]))
		i++;
	if (!i)
		return ;
	j = 0;
	while (j < i)
	{
		new_node = create_env_node(NULL, NULL, 0);
		validate_malloc(data, new_node, NULL);
		split_env_entry(data, (*stack)->node->argv[j], new_node);
		j++;
	}
	(*stack)->node->argv = ft_shrink_split((*stack)->node->argv,
			j - 1, ft_splitlen((*stack)->node->argv) - 1);
}

void	executor_cleanup(t_data *data, t_stack **stack, char *cmd)
{
	undo_duplicate_std(0);
	close_all_open_redir_ends(data);
	close_all_pipe_ends(stack);
	free(cmd);
	free_stack(stack);
	free_all(data);
}
