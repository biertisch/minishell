/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_tilde.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:57:45 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/03 15:41:43 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_tilde(int quote_status, char *arg)
{
	return (!quote_status && arg && (!ft_strcmp(arg, "~")
		|| !ft_strncmp(arg, "~/", 2) || !ft_strncmp(arg, "~+", 2)
		|| !ft_strncmp(arg, "~-", 2)));
}

int	expand_tilde(t_data *data, char **arg, t_arg_info *info)
{
	char	*new_arg;
	int		new_len;

	if (!arg || !*arg || !info)
		return (0);
	while (has_tilde(info->quote_map[0], *arg))
	{
		update_tilde_key_value(data, *arg, info);
		if (!*(info->value))
			break ;
		info->value_len = ft_strlen(info->value);
		new_len = ft_strlen(*arg) - info->key_len + info->value_len;
		rebuild_quote_map(data, info, 0, new_len);
		rebuild_expand_map(data, info, 0, new_len);
		new_arg = apply_expansion(*arg, info, 0, new_len);
		validate_malloc(data, new_arg, NULL);
		free(*arg);
		*arg = new_arg;
	}
	return (0);
}

int	update_tilde_key_value(t_data *data, char *arg, t_arg_info *info)
{
	free(info->key);
	info->key = get_tilde_key(arg);
	validate_malloc(data, info->key, NULL);
	info->key_len = ft_strlen(info->key);
	free(info->value);
	info->value = get_tilde_value(data, info->key);
	validate_malloc(data, info->value, NULL);
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

// PREVIOUS VERSION
// int	expand_tilde(t_data *data, char **arg)
// {
// 	char	*value;
// 	char	*key;

// 	if (!arg || !*arg)
// 		return (0);
// 	while (has_tilde(*arg))
// 	{
// 		key = get_tilde_key(*arg);
// 		validate_malloc(data, key, NULL);
// 		value = get_tilde_value(data, key);
// 		if (!value)
// 		{
// 			free(key);
// 			break ;
// 		}
// 		*arg = update_arg(*arg, 0, key, value);
// 		validate_malloc(data, *arg, key);
// 		free(key);
// 	}
// 	return (0);
// }

// char	*replace_tilde_value(char *arg, int key_len, char *value)
// {
// 	char	*new_arg;
// 	int		new_len;
// 	int		value_len;

// 	value_len = ft_strlen(value);
// 	new_len = ft_strlen(arg) - key_len + value_len;
// 	new_arg = malloc(sizeof(char) * (new_len + 1));
// 	if (!new_arg)
// 		return (NULL);
// 	ft_strlcpy(new_arg, value, value_len + 1);
// 	ft_strlcat(new_arg, arg + key_len, new_len + 1);
// 	free(arg);
// 	return (new_arg);
// }
