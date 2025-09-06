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

static void	split_env_entry(t_data *data, char *entry, t_env *node)
{
	char	*equal;

	equal = ft_strchr(entry, '=');
	if (equal)
	{
		node->key = ft_substr(entry, 0, equal - entry);
		validate_malloc_env(data, node->key, node);
		node->value = ft_strdup(equal + 1);
		validate_malloc_env(data, node->value, node);
	}
	else
	{
		node->key = ft_strdup(entry);
		validate_malloc(data, node->key, node);
	}
}

//helpful to implement built-in export and unset
void	envp_to_list(t_data *data, char **envp)
{
	int		i;
	t_env	*node;

	if (!envp || !*envp)
		return ;
	i = 0;
	while (envp[i])
	{
		node = create_env_node();
		validate_malloc(data, node, NULL);
		split_env_entry(data, envp[i], node);
		if (!node->key[0])
		{
			report_error("invalid environment variable key", INTERNAL_ERR);
			free_env_node(&node);
			i++;
			continue ;
		}
		add_env_node(&data->env_list, node);
		i++;
	}
}

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

//helpful to pass updated env to execve()
void	env_list_to_array(t_data *data)
{
	t_env	*trav;
	int		counter;
	int		i;

	free_string_array(&data->env);
	trav = data->env_list;
	counter = count_nodes(trav);
	data->env = malloc(sizeof(char *) * (counter + 1));
	validate_malloc(data, data->env, NULL);
	i = 0;
	while (trav)
	{
		data->env[i] = join_key_value(trav);
		validate_malloc(data, data->env[i], NULL);
		trav = trav->next;
		i++;
	}
	data->env[i] = NULL;
}
