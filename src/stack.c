/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:15:44 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/02 15:15:45 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_stack	*create_stack(t_data *data)
{
	t_stack	*head;

	head = malloc(sizeof(t_stack));
	validate_malloc(data, head, NULL);
	head->phase = ENTERED;
	head->type = (data->parser_tree)->type;
	head->node = data->parser_tree;
	head->in_fd = STDIN_FILENO;
	head->out_fd = STDOUT_FILENO;
	head->next = NULL;
	return (head);
}
