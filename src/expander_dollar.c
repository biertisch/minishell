/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:43:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/03 22:13:31 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dollar_expansion(char *arg, int *quote_map, int start)
{
	return (quote_map[start] != 1 && arg[start] == '$' && arg[start + 1]
		&& quote_map[start] == quote_map[start + 1]
		&& (ft_isalpha(arg[start + 1]) || arg[start + 1] == '_'
		|| arg[start + 1] == '?'));
}

int	expand_dollar(t_data *data, char **arg, t_arg_info *info)
{
	int	i;

	if (!arg || !*arg || !info)
		return (0);
	if (info->total_len < 1)
		info->expand_map = ft_calloc(1, sizeof(int));
	else
		info->expand_map = ft_calloc(info->total_len, sizeof(int));
	validate_malloc(data, info->expand_map, NULL);
	i = 0;
	while ((*arg) && (*arg)[i])
	{
		if (is_dollar_expansion(*arg, info->quote_map, i))
		{
			if ((*arg)[i + 1] == '?')
				*arg = expand_exit_status(data, *arg, i, info);
			else
				*arg = expand_variable(data, *arg, i, info);
			i = -1;
		}
		i++;
	}
	return (0);
}

char	*expand_exit_status(t_data *data, char *arg, int i, t_arg_info *info)
{
	char	*new_arg;

	free(info->key);
	info->key = ft_strdup("$?");
	validate_malloc(data, info->key, NULL);
	info->key_len = ft_strlen(info->key);
	free(info->value);
	info->value = ft_itoa(data->exit_status);
	validate_malloc(data, info->value, NULL);
	info->value_len = ft_strlen(info->value);
	info->total_len = ft_strlen(arg) - info->key_len + info->value_len;
	rebuild_quote_map(data, info, i);
	rebuild_expand_map(data, info, i, 1);
	new_arg = apply_expansion(arg, info, i);
	validate_malloc(data, new_arg, NULL);
	free(arg);
	return (new_arg);
}

char	*expand_variable(t_data *data, char *arg, int i, t_arg_info *info)
{
	char	*new_arg;
	char	*tmp;

	free(info->key);
	info->key = get_env_key(arg, info->quote_map, i);
	validate_malloc(data, info->key, NULL);
	info->key_len = ft_strlen(info->key);
	free(info->value);
	tmp = get_env_value(data->env_list, info->key + 1);
	if (!tmp)
		info->value = ft_strdup("");
	else
		info->value = ft_strdup(tmp);
	validate_malloc(data, info->value, NULL);
	info->value_len = ft_strlen(info->value);
	info->total_len = ft_strlen(arg) - info->key_len + info->value_len;
	rebuild_quote_map(data, info, i);
	rebuild_expand_map(data, info, i, 1);
	new_arg = apply_expansion(arg, info, i);
	validate_malloc(data, new_arg, NULL);
	free(arg);
	return (new_arg);
}

int	rebuild_quote_map(t_data *data, t_arg_info *info, int start)
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

int	handle_empty_quote_map(t_data *data, t_arg_info *info, int start, int old_len)
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

int	rebuild_expand_map(t_data *data, t_arg_info *info, int start, int type)
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

int	handle_empty_expand_map(t_data *data, t_arg_info *info)
{
	int	*new_map;

	new_map = ft_calloc(1, sizeof(int));
	validate_malloc(data, new_map, NULL);
	new_map[0] = 1;
	free(info->expand_map);
	info->expand_map = new_map;
	return (0);
}

char *apply_expansion(char *src, t_arg_info *info, int start)
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

// PREVIOUS VERSION
// int	is_dollar_expansion(char quote, char *arg)
// {
// 	return (quote != '\'' && arg[0] == '$' && arg[1]
// 		&& (ft_isalpha(arg[1]) || arg[1] == '_' || arg[1] == '?'));
// }

// int	expand_dollar(t_data *data, char **arg, t_arg_info arg_info)
// {
// 	int		i;
// 	char	quote;

// 	if (!arg || !*arg)
// 		return (0);
// 	quote = 0;
// 	i = 0;
// 	while ((*arg) && (*arg)[i])
// 	{
// 		update_quote_status((*arg)[i], &quote);
// 		if (is_dollar_expansion(quote, *arg + i))
// 		{
// 			if ((*arg)[i + 1] == '?')
// 				*arg = expand_exit_status(data, *arg, i);
// 			else
// 				*arg = expand_variable(data, *arg, i);
// 			i = -1;
// 		}
// 		i++;
// 	}
// 	return (0);
// }

// char	*expand_exit_status(t_data *data, char *old_arg, int i)
// {
// 	char	*value;
// 	char	*new_arg;

// 	value = ft_itoa(data->exit_status);
// 	validate_malloc(data, value, NULL);
// 	new_arg = update_arg(old_arg, i, "$?", value);
// 	validate_malloc(data, new_arg, value);
// 	free(value);
// 	return (new_arg);
// }

// char	*expand_variable(t_data *data, char *old_arg, int i)
// {
// 	char	*key;
// 	char	*value;
// 	char	default_value[1];
// 	char	*new_arg;

// 	default_value[0] = '\0';
// 	key = get_env_key(old_arg + i);
// 	validate_malloc(data, key, NULL);
// 	value = get_env_value(data->env_list, key + 1);
// 	if (!value)
// 		value  = default_value;
// 	new_arg = update_arg(old_arg, i, key, value);
// 	validate_malloc(data, new_arg, key);
// 	free(key);
// 	return (new_arg);
// }


// char	**expand_variable(t_data *data, char **arg, int index, int i)
// {
// 	char	*key;
// 	char	*value;

// 	key = get_env_key(arg[index] + i + 1);
// 	validate_malloc(data, key, NULL);
// 	value = get_env_value(data->env_list, key);
// 	if (arg[index][0] == '"' || (value && !ft_strchr(value, ' ')))
// 	{
// 		arg[index] = update_arg(arg[index], i, key, value);
// 		validate_malloc(data, arg[index], key);
// 	}
// 	else
// 	{
// 		arg = update_argv_dollar(arg, index, value);
// 		validate_malloc(data, arg, key);
// 	}
// 	free(key);
// 	return (arg);
// }

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

// char	*update_arg(char *arg, int i, char *key, char *value)
// {
// 	char	*new_arg;
// 	int		new_len;

// 	new_len = ft_strlen(arg) - ft_strlen(key) + ft_strlen(value);
// 	new_arg = ft_calloc(new_len + 1, sizeof(char));
// 	if (!new_arg)
// 		return (NULL);
// 	if (new_len != 0)
// 	{
// 		ft_strlcpy(new_arg, arg, i + 1);
// 		if (value)
// 			ft_strlcat(new_arg, value, new_len + 1);
// 		ft_strlcat(new_arg, arg + i + ft_strlen(key), new_len + 1);
// 	}
// 	free(arg);
// 	return (new_arg);
// }
