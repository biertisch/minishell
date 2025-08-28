/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:42:52 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/20 17:42:52 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//GENERAL PURPOSE:
//helper functions to handle singly linked list with environment variables

t_env	*create_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

void	add_env_node(t_env **head, t_env *new_node)
{
	t_env	*last_node;

	if (!head || !new_node)
		return ;
	if (!*head)
		*head = new_node;
	else
	{
		last_node = get_last_env_node(*head);
		last_node->next = new_node;
	}
}

t_env	*get_last_env_node(t_env *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

void	free_env_node(t_env **node)
{
	if (!node || !*node)
		return ;
	free((*node)->key);
	free((*node)->value);
	free(*node);
	*node = NULL;
}

void	free_env_list(t_env **head)
{
	t_env	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = (*head)->next;
		free_env_node(head);
		*head = tmp;
	}
}
