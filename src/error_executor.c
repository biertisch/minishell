/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:47:52 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/08 11:48:12 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	validate_malloc_execute(t_data *data, t_stack **stack,
	void *ptr, void *to_free)
{
	if (ptr)
		return ;
	undo_duplicate_std(0);
	close_all_open_redir_ends(data);
	close_all_pipe_ends(stack);
	system_error(strerror(errno), "malloc");
	if (to_free)
		free(to_free);
	error_exit(data, stack);
}

int	validate_fork(t_data *data, t_stack **stack)
{
	int	i;

	i = 0;
	if (has_node_type_ancestor(*stack, NODE_PIPE))
	{
		while (has_node_type_ancestor(*stack, NODE_PIPE))
		{
			pop(stack);
			i++;
		}
		data->exit_status = 1;
	}
	else
		(*stack)->exit_status = 0;
	system_error(strerror(errno), "fork");
	return (i);
}

int	validate_pipe(int pipe_res, t_stack **stack)
{
	if (pipe_res == 0)
		return (0);
	close_all_pipe_ends(stack);
	(*stack)->phase = DONE;
	(*stack)->exit_status = 1;
	system_error(strerror(errno), "pipe");
	return (1);
}

int	env_too_long(t_data *data, t_stack **stack)
{
	long	count;
	t_env	*env;
	long	len;

	env = data->env_list;
	count = 0;
	while (env)
	{
		len = ft_strlen(env->key) + 2;
		if (len > MAX_ARG_STRLEN)
			return (env_not_good_very_bad(stack));
		count += (len);
		len = ft_strlen(env->value) + 2;
		if (len > MAX_ARG_STRLEN)
			return (env_not_good_very_bad(stack));
		count += (len);
		env = env->next;
	}
	if (count > ARG_MAX)
		return (env_not_good_very_bad(stack));
	return (0);
}

int	env_not_good_very_bad(t_stack **stack)
{
	internal_error(INT_ERR_12, "/usr/bin/env", NULL);
	(*stack)->exit_status = 126;
	return (126);
}
