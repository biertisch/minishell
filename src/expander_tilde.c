/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_tilde.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:57:45 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 22:00:18 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_tilde(t_data *data, char **arg, t_metadata *info)
{
	char	*new_arg;

	if (!arg || !*arg || !info)
		return (0);
	while (has_tilde(*arg, info->quote_map[0]))
	{
		update_tilde_key_value(data, *arg, info);
		if (!*(info->value))
			break ;
		info->total_len = ft_strlen(*arg) - info->key_len + info->value_len;
		rebuild_quote_map(data, info, 0);
		rebuild_expand_map(data, info, 0, 2);
		new_arg = apply_expansion(*arg, info, 0);
		validate_malloc(data, new_arg, NULL);
		free(*arg);
		*arg = new_arg;
	}
	return (0);
}

int	has_tilde(char *arg, int quote_status)
{
	return (!quote_status
		&& arg
		&& (!ft_strcmp(arg, "~")
			|| !ft_strncmp(arg, "~/", 2)
			|| !ft_strncmp(arg, "~+", 2)
			|| !ft_strncmp(arg, "~-", 2)));
}

int	update_tilde_key_value(t_data *data, char *arg, t_metadata *info)
{
	free(info->key);
	free(info->value);
	info->key = get_tilde_key(arg);
	validate_malloc(data, info->key, NULL);
	info->value = get_tilde_value(data, info->key);
	validate_malloc(data, info->value, NULL);
	info->key_len = ft_strlen(info->key);
	info->value_len = ft_strlen(info->value);
	return (0);
}

char	*get_tilde_key(char *arg)
{
	if (!ft_strncmp(arg, "~+", 2))
		return (ft_strdup("~+"));
	else if (!ft_strncmp(arg, "~-", 2))
		return (ft_strdup("~-"));
	else
		return (ft_strdup("~"));
}

char	*get_tilde_value(t_data *data, char *key)
{
	char	*value;

	if (!ft_strcmp(key, "~+"))
		value = get_env_value(data->env_list, "PWD");
	else if (!ft_strcmp(key, "~-"))
		value = get_env_value(data->env_list, "OLDPWD");
	else
		value = get_env_value(data->env_list, "HOME");
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}
