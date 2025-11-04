/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:43:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 16:25:45 by beatde-a         ###   ########.fr       */
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

int	expand_dollar(t_data *data, char **arg, t_metadata *info)
{
	int	i;

	if (!arg || !*arg || !info)
		return (0);
	init_expand_metadata(data, info);
	i = 0;
	while ((*arg) && (*arg)[i])
	{
		if (is_dollar_expansion(*arg, info->quote_map, i))
		{
			*arg = expand_variable(data, *arg, info, i);
			i = -1;
		}
		i++;
	}
	return (0);
}

int	init_expand_metadata(t_data *data, t_metadata *info)
{
	free(info->expand_map);
	if (info->total_len < 1)
		info->expand_map = ft_calloc(1, sizeof(int));
	else
		info->expand_map = ft_calloc(info->total_len, sizeof(int));
	validate_malloc(data, info->expand_map, NULL);
	free(info->key);
	info->key = NULL;
	free(info->value);
	info->value = NULL;
	return (0);
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

int	rebuild_quote_map(t_data *data, t_metadata *info, int start)
{
	int	*new_map;
	int	old_len;
	int	status;
	int	i;
	int	j;

	old_len = info->total_len + info->key_len - info->value_len;
	if (info->total_len == 0)
		return (handle_empty_quote_map(data, info, start, old_len));
	new_map = ft_calloc(info->total_len, sizeof(int));
	validate_malloc(data, new_map, NULL);
	i = 0;
	j = 0;
	while (i < start)
		new_map[i++] = info->quote_map[j++];
	status = get_quote_status(info->quote_map, j, old_len);
	while (i < start + info->value_len)
		new_map[i++] = status;
	j += info->key_len;
	while (i < info->total_len && j < old_len)
		new_map[i++] = info->quote_map[j++];
	free(info->quote_map);
	info->quote_map = new_map;
	return (0);
}

int	handle_empty_quote_map(t_data *data, t_metadata *info, int start,
	int old_len)
{
	int	*new_map;

	new_map = ft_calloc(1, sizeof(int));
	validate_malloc(data, new_map, NULL);
	if (start < old_len)
		new_map[0] = info->quote_map[start];
	else
		new_map[0] = 0;
	free(info->quote_map);
	info->quote_map = new_map;
	return (0);
}

int	get_quote_status(int *quote_map, int start, int len)
{
	if (start < len)
		return (quote_map[start]);
	return (0);
}

int	rebuild_expand_map(t_data *data, t_metadata *info, int start, int type)
{
	int	*new_map;
	int	old_len;
	int	i;
	int	j;

	if (info->total_len == 0)
		return (handle_empty_expand_map(data, info));
	old_len = info->total_len + info->key_len - info->value_len;
	new_map = ft_calloc(info->total_len, sizeof(int));
	validate_malloc(data, new_map, NULL);
	i = 0;
	j = 0;
	while (info->expand_map && i < start && i < old_len)
		new_map[i++] = info->expand_map[j++];
	i = start;
	while (i < start + info->value_len && i < info->total_len)
		new_map[i++] = type;
	j += info->key_len;
	while (info->expand_map && i < info->total_len && j < old_len)
		new_map[i++] = info->expand_map[j++];
	free(info->expand_map);
	info->expand_map = new_map;
	return (0);
}

int	handle_empty_expand_map(t_data *data, t_metadata *info)
{
	int	*new_map;

	new_map = ft_calloc(1, sizeof(int));
	validate_malloc(data, new_map, NULL);
	new_map[0] = 1;
	free(info->expand_map);
	info->expand_map = new_map;
	return (0);
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
