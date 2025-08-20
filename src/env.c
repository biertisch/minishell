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

static void	split_env_entry(char *entry, char **key, char **value)
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
		if (!key || (!value && ft_strchr(envp[i], '=')))
			//error		
		new_node = create_env_node(key, value);
		if (!new_node)
			//error
		add_env_node(&data->env_list, new_node);
		i++;
	}
}

//envp_to_array

//get_env_value

//set_env_value

//unset_env
