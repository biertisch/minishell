/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_tilde.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:57:45 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/03 14:42:30 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	has_tilde(char *arg)
// {
// 	if (!arg || *arg != '~')
// 		return (0);
// 	return (1);
// }

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

// char	*get_tilde_key(char *arg)
// {
// 	if (!ft_strncmp(arg, "~+", 2))
// 		return (ft_strdup("~+"));
// 	else if (!ft_strncmp(arg, "~-", 2))
// 		return (ft_strdup("~-"));
// 	else
// 		return (ft_strdup("~"));
// }

// char	*get_tilde_value(t_data *data, char *key)
// {
// 	if (!ft_strcmp(key, "~+"))
// 		return (get_env_value(data->env_list, "PWD"));
// 	else if (!ft_strcmp(key, "~-"))
// 		return (get_env_value(data->env_list, "OLDPWD"));
// 	else
// 		return (get_env_value(data->env_list, "HOME"));
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
