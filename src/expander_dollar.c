/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:43:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 21:59:26 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dollar_expansion(char *input, int *quote_map, int start)
{
	return (quote_map[start] != 1
		&& input[start] == '$'
		&& input[start + 1]
		&& quote_map[start] == quote_map[start + 1]
		&& (ft_isalpha(input[start + 1])
			|| input[start + 1] == '_'
			|| input[start + 1] == '?'));
}

int	get_key_value(t_data *data, char *arg, t_metadata *info, int i)
{
	char	*tmp;

	free(info->key);
	free(info->value);
	if (arg[i + 1] == '?')
	{
		info->key = ft_strdup("$?");
		validate_malloc(data, info->key, NULL);
		info->value = ft_itoa(data->exit_status);
		validate_malloc(data, info->value, NULL);
	}
	else
	{
		info->key = get_env_key(arg, info->quote_map, i);
		validate_malloc(data, info->key, NULL);
		tmp = get_env_value(data->env_list, info->key + 1);
		if (!tmp)
			info->value = ft_strdup("");
		else
			info->value = ft_strdup(tmp);
		validate_malloc(data, info->value, NULL);
	}
	info->key_len = ft_strlen(info->key);
	info->value_len = ft_strlen(info->value);
	return (0);
}

char	*get_env_key(char *arg, int *quote_map, int start)
{
	char	*key;
	int		len;
	int		i;
	int		status;

	i = start;
	status = quote_map[i];
	if (arg[i] == '$')
		i++;
	while (arg[i] && quote_map[i] == status
		&& (ft_isalnum(arg[i]) || arg[i] == '_'))
		i++;
	len = i - start;
	key = malloc(sizeof(char) * (len + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, arg + start, len + 1);
	return (key);
}

char	*expand_variable(t_data *data, char *arg, t_metadata *info, int i)
{
	char	*new_arg;

	get_key_value(data, arg, info, i);
	info->total_len = ft_strlen(arg) - info->key_len + info->value_len;
	rebuild_quote_map(data, info, i);
	rebuild_expand_map(data, info, i, 1);
	new_arg = apply_expansion(arg, info, i);
	validate_malloc(data, new_arg, NULL);
	free(arg);
	return (new_arg);
}

char	*apply_expansion(char *src, t_metadata *info, int start)
{
	char	*dest;

	dest = ft_calloc(info->total_len + 1, sizeof(char));
	if (!dest)
		return (NULL);
	if (info->total_len < 1)
		return (dest);
	ft_strlcpy(dest, src, start + 1);
	if (info->value)
		ft_strlcat(dest, info->value, info->total_len + 1);
	ft_strlcat(dest, src + start + info->key_len, info->total_len + 1);
	return (dest);
}
