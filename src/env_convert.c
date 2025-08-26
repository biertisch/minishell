/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:51:27 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/21 11:51:27 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*PURPOSE: split envp entry into key and value
helper to envp_to_list()*/
static void	split_env_entry(const char *entry, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(entry, '=');
	if (equal)
	{
		*key = ft_substr(entry, 0, equal - entry);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(entry);
		*value = NULL;
	}
}

/*PURPOSE: convert envp to singly linked list
helpful to implement built-in export and unset*/
void	envp_to_list(t_data *data, char **envp)
{
	int		i;
	char	*key;
	char	*value;
	t_env	*new_node;

	i = 0;
	while (envp[i])
	{
		split_env_entry(envp[i], &key, &value);
		if (!key || !*key)
		{
			report_error("invalid environment variable key", INTERNAL_ERR);
			continue ;
		}
		new_node = create_env_node(key, value);
		validate_malloc(data, new_node);
		add_env_node(&data->env_list, new_node);
		i++;
	}
}

/*PURPOSE: count nodes of env_list
helper to env_list_to_array()*/
static int	count_nodes(t_env *head)
{
	int	counter;

	counter = 0;
	while (head)
	{
		counter++;
		head = head->next;
	}
	return (counter);
}

/*PURPOSE: join env key and value into a single string
helper to env_list_to_array()*/
static char	*join_key_value(t_env *node)
{
	char	*res;
	char	*tmp;

	if (!node->value)
		return (ft_strdup(node->key));
	tmp = ft_strjoin(node->key, "=");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, node->value);
	free(tmp);
	return (res);
}

/*PURPOSE: convert env list back to an array of strings
follows same format as envp
necessary to pass updated env to execve()*/
void	env_list_to_array(t_data *data)
{
	t_env	*trav;
	int		counter;
	int		i;

	free_str_array(&data->env);
	trav = data->env_list;
	counter = count_nodes(trav);
	data->env = malloc(sizeof(char *) * (counter + 1));
	validate_malloc(data, data->env);
	i = 0;
	while (trav)
	{
		data->env[i] = join_key_value(trav);
		validate_malloc(data, data->env[i]);
		trav = trav->next;
		i++;
	}
	data->env[i] = NULL;
}
