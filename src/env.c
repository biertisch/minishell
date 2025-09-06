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

<<<<<<< HEAD
//useful to implement built-in export
=======
//used in variable expansion
char	*replace_key_value(char *arg, int i, char *key, char *value)
{
	char	*new_arg;
	int		new_len;

	new_len = ft_strlen(arg) - ft_strlen(key) - 1 + ft_strlen(value);
	new_arg = malloc(sizeof(char) * (new_len + 1));
	if (!new_arg)
		return (NULL);
	ft_strlcpy(new_arg, arg, i + 1);
	ft_strlcat(new_arg, value, new_len + 1);
	ft_strlcat(new_arg, arg + i + 1 + ft_strlen(key), new_len + 1);
	free(arg);
	return (new_arg);
}

//used in variable expansion
char	*get_env_key(char *arg)
{
	char	*key;
	int		key_len;

	key_len = 0;
	while (arg[key_len] && (ft_isalnum(arg[key_len]) || arg[key_len] == '_'))
		key_len++;
	key = malloc(sizeof(char) * (key_len + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, arg, key_len + 1);
	return (key);
}

//used variable expansion and built-in export
>>>>>>> origin/dev
char	*get_env_value(t_env *head, char *key)
{
	while (head)
	{
		if (!ft_strcmp(head->key, key))
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

<<<<<<< HEAD
//useful to implement built-in export
=======
//used in built-in export
>>>>>>> origin/dev
void	set_env_value(t_env *head, char *key, char *new_value)
{
	while (head)
	{
		if (!ft_strcmp(head->key, key))
		{
			free(head->value);
<<<<<<< HEAD
			head->value = ft_strdup(new_value); //remove strdup if new_value is already malloc'ed
=======
			head->value = new_value; //strdup if new_value is not malloc'ed
>>>>>>> origin/dev
			return ;
		}
		head = head->next;
	}
}

<<<<<<< HEAD
//useful to implement built-in unset
=======
//used in built-in unset
>>>>>>> origin/dev
void	unset_env(t_env **head, char *key)
{
	t_env	*trav;
	t_env	*tmp;

	if (!head || !*head)
		return ;
	while (*head && !ft_strcmp((*head)->key, key))
	{
		tmp = *head;
		*head = (*head)->next;
		free_env_node(&tmp);
	}
	trav = *head;
	while (trav && trav->next)
	{
		if (!ft_strcmp(trav->next->key, key))
		{
			tmp = trav->next;
			trav->next = trav->next->next;
			free_env_node(&tmp);
		}
		else
			trav = trav->next;
	}
}
