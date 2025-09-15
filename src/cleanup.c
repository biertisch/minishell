/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:37:43 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/21 12:37:43 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_string_array(char ***arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i])
		free((*arr)[i++]);
	free(*arr);
	*arr = NULL;
}

//called after every iteration of main loop
void	free_command_data(t_data *data)
{
	free(data->input);
	data->input = NULL;
	free_lexer_list(&data->lexer_list);
	free_parser_tree(&data->parser_tree);
}

void	free_all(t_data *data)
{
	free_string_array(&data->env);
	free_env_list(&data->env_list);
	free_command_data(data);
	rl_clear_history();
}

void	free_stack(t_stack *stack)
{
	t_stack	*next;
	t_stack	*old_next;

	old_next = stack;
	next = stack->next;
	while (next)
	{
		free(old_next);
		old_next = next;
		next = next->next;
	}
	free(old_next);
}
