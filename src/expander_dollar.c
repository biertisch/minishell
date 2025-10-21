/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:43:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/21 22:48:30 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*update_arg(char *arg, int i, char *key, char *value)
{
	char	*new_arg;
	int		new_len;

	new_len = ft_strlen(arg) - ft_strlen(key) - 1 + ft_strlen(value);
	new_arg = malloc(sizeof(char) * (new_len + 1));
	if (!new_arg)
		return (NULL);
	ft_strlcpy(new_arg, arg, i + 1);
	if (value)
		ft_strlcat(new_arg, value, new_len + 1);
	ft_strlcat(new_arg, arg + i + 1 + ft_strlen(key), new_len + 1);
	free(arg);
	return (new_arg);
}

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

void	expand_exit_status(t_data *data, char **arg, int i)
{
	char	*value;

	value = ft_itoa(data->exit_status);
	validate_malloc(data, value, NULL);
	*arg = update_arg(*arg, i, "?", value);
	validate_malloc(data, *arg, value);
	free(value);
}

static char	**expand_variable(t_data *data, char **arg, int index, int i)
{
	char	*key;
	char	*value;

	key = get_env_key(arg[index] + i + 1);
	validate_malloc(data, key, NULL);
	value = get_env_value(data->env_list, key);
	if ((value || arg[index][0] == '"')
		&& (arg[index][0] == '"' || !ft_strchr(value, ' ')))
	{
		arg[index] = update_arg(arg[index], i, key, value);
		validate_malloc(data, arg[index], key);
	}
	else
	{
		arg = update_argv_dollar(arg, index, value);
		validate_malloc(data, arg, key);
	}
	free(key);
	return (arg);
}

char	**expand_dollar(t_data *data, char **arg, int index)
{
	int	i;

	if (!arg || !arg[index] || arg[index][0] == '\'')
		return (arg);
	i = 0;
	while (arg[index] && arg[index][i])
	{
		if (arg[index][i] == '$' && arg[index][i + 1]
			&& arg[index][i + 1] != ' ' && arg[index][i + 1] != '"')
		{
			if (arg[index][i + 1] == '?')
				expand_exit_status(data, &arg[index], i);
			else
				arg = expand_variable(data, arg, index, i);
			i = -1;
		}
		i++;
	}
	return (arg);
}
