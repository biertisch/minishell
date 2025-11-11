/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:37:43 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/11 21:50:46 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_data *data)
{
	ft_splitfree(data->env);
	data->env = NULL;
	free_env_list(&data->env_list);
	free_command_data(data);
	rl_clear_history();
	free(data->prompt);
}

//called after every iteration of main loop
void	free_command_data(t_data *data)
{
	ft_lstclear(&data->open_redirs, free);
	free(data->input);
	data->input = NULL;
	free_lexer_list(&data->lexer_list);
	free_stack(&data->stack);
	free_parser_tree(&data->parser_tree);
}

void	free_stack(t_stack **stack)
{
	t_stack	*next;
	t_stack	*old_next;

	if (!stack || !*stack)
		return ;
	old_next = *stack;
	next = (*stack)->next;
	while (next)
	{
		free(old_next);
		old_next = next;
		next = next->next;
	}
	free(old_next);
	*stack = NULL;
}

void	free_metadata(t_metadata **info, int size)
{
	int	i;

	if (!info || !*info)
		return ;
	i = 0;
	while (i < size)
	{
		free((*info)[i].quote_map);
		free((*info)[i].expand_map);
		free((*info)[i].key);
		free((*info)[i].value);
		i++;
	}
	free(*info);
	*info = NULL;
}
