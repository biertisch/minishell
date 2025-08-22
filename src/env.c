/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:15 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/20 10:38:15 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env_value(t_env *head, char *key)
{
	while (head)
	{
		if (ft_strcmp(head->key, key) == 0)
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

void	set_env_value(t_env *head, char *key, char *new_value)
{
	while (head)
	{
		if (ft_strcmp(head->key, key) == 0)
		{
			free(head->value);
			head->value = ft_strdup(new_value);
			return ;
		}
		head = head->next;
	}
}

void	unset_env(t_env **head, char *key)
{
	t_env	*trav;
	t_env	*tmp;

	if (!head || !*head)
		return ;
	while (*head && ft_strcmp((*head)->key, key) == 0)
	{
		tmp = *head;
		*head = (*head)->next;
		free_env_node(&tmp);
	}
	trav = *head;
	while (trav && trav->next)
	{
		if (ft_strcmp(trav->next->key, key) == 0)
		{
			tmp = trav->next;
			trav->next = trav->next->next;
			free_env_node(&tmp);
		}
		else
			trav = trav->next;
	}
}
