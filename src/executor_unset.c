/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:39:13 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:11:34 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_unset(t_data *data, t_stack **stack)
{
	t_unset_vars	v;
	int				exit_status;

	execute_unset_path(data, stack);
	v.i = get_first_command(data, stack) + 1;
	exit_status = validate_unset_export(data, stack, v.i - 1, "unset");
	while (!exit_status && (*stack)->node->argv[v.i])
	{
		v = get_begginer_u_v(data, v.i);
		while (v.second && *v.second)
		{
			v.third = &((*v.second)->next);
			if (!ft_strcmp((*v.second)->key, (*stack)->node->argv[v.i])
				&& found_victim(data, &v))
				break ;
			v.first = v.second;
			v.second = v.third;
			v.third = NULL;
		}
		v.i++;
	}
	(*stack)->exit_status = exit_status;
	undo_duplicate_std(1);
	execute_builtin_check_for_pipe(data, stack);
	return (0);
}

t_unset_vars	get_begginer_u_v(t_data *data, int i)
{
	t_unset_vars	v;

	v.i = i;
	v.first = NULL;
	v.second = &(data->env_list);
	v.third = NULL;
	v.victim = NULL;
	v.next = NULL;
	return (v);
}

int	found_victim(t_data *data, t_unset_vars *v)
{
	v->victim = *(v->second);
	v->next = *(v->third);
	if (v->first)
		(*(v->first))->next = v->next;
	else
		data->env_list = v->next;
	free_env_node(&(v->victim));
	return (1);
}

int	validate_unset_export(t_data *data, t_stack **stack, int cmd_i, char *cmd)
{
	int	exit_status;

	duplicate_std();
	if (!has_node_type_ancestor(*stack, NODE_PIPE))
		handle_redirects(data, stack, NULL, (*stack)->node->redir);
	exit_status = 0;
	if ((*stack)->node->argv[cmd_i + 1]
		&& *(*stack)->node->argv[cmd_i + 1] == '-')
	{
		internal_error(INT_ERR_2, cmd, (*stack)->node->argv[cmd_i + 1]);
		print_builtin_usage(cmd);
		exit_status = 2;
	}
	return (exit_status);
}
