/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:13:12 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/21 13:13:12 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_lexer_node(t_token_type type, char *value)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

void	add_lexer_node(t_token **head, t_token *new_node)
{
	t_token	*last_node;

	if (!head || !new_node)
		return ;
	if (!*head)
		*head = new_node;
	else
	{
		last_node = get_last_lexer_node(*head);
		last_node->next = new_node;
	}
}

t_token	*get_last_lexer_node(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

void	free_lexer_node(t_token **node)
{
	if (!node || !*node)
		return ;
	free((*node)->value);
	free(*node);
	*node = NULL;
}

void	free_lexer_list(t_token **head)
{
	t_token	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = (*head)->next;
		free_lexer_node(head);
		*head = tmp;
	}
}
