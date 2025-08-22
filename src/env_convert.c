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
		//if (!key || (!value && ft_strchr(envp[i], '=')))
			//error		
		new_node = create_env_node(key, value);
		//if (!new_node)
			//error
		add_env_node(&data->env_list, new_node);
		i++;
	}
}

static size_t	count_nodes(t_env *head)
{
	size_t	counter;

	counter = 0;
	while (head)
	{
		counter++;
		head = head->next;
	}
	return (counter);
}

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

void	env_list_to_array(t_data *data)
{
	t_env	*trav;
	size_t	counter;
	int		i;

	free_str_array(&data->env);
	trav = data->env_list;
	counter = count_nodes(trav);
	data->env = malloc(sizeof(char *) * (counter + 1));
	//if (!data->env)
		//error
	i = 0;
	while (trav)
	{
		data->env[i] = join_key_value(trav);
		//if (!data->env[i])
			//error
		trav = trav->next;
		i++;
	}
	data->env[i] = NULL;
}
