/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:43:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/04 12:43:51 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	expand_exit_status(t_data *data, char **arg, int i)
{
	char	*value;

	value = ft_itoa(data->exit_status);
	validate_malloc(data, value, NULL);
	*arg = replace_key_value(*arg, i, "?", value);
	validate_malloc(data, *arg, value);
	free(value);
}

static void	expand_variable(t_data *data, char **arg, int i)
{
	char	*key;
	char	*value;

	key = get_env_key((*arg) + i + 1);
	validate_malloc(data, key, NULL);
	value = get_env_value(data->env_list, key);
	if (!value)
		value = "";
	*arg = replace_key_value(*arg, i, key, value);
	validate_malloc(data, *arg, key);
	free(key);
}

void	expand_dollar(t_data *data, char **arg)
{
	int	i;

	i = 0;
	while ((*arg)[i])
	{
		if ((*arg)[i] == '$')
		{
			if ((*arg)[i + 1] == '?')
				expand_exit_status(data, arg, i);
			else
				expand_variable(data, arg, i);
			i = -1;
		}
		i++;
	}
}
