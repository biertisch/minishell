/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:43:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/03 14:40:17 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dollar_expansion(int quote_status, char *arg)
{
	return (quote_status != 1 && arg[0] == '$' && arg[1]
		&& (ft_isalpha(arg[1]) || arg[1] == '_' || arg[1] == '?'));
}

int	expand_dollar(t_data *data, char **arg, t_arg_info *info)
{
	int		i;

	if (!arg || !*arg | !info)
		return (0);
	i = 0;
	while ((*arg) && (*arg)[i])
	{
		if (is_dollar_expansion(info->quote_map[i], *arg + i))
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
	int		new_len;

	free(info->key);
	info->key = ft_strdup("$?");
	validate_malloc(data, info->key, NULL);
	info->key_len = ft_strlen(info->key);
	free(info->value);
	info->value = ft_itoa(data->exit_status);
	validate_malloc(data, info->value, NULL);
	info->value_len = ft_strlen(info->value);
	new_len = ft_strlen(arg) - info->key_len + info->value_len;
	rebuild_quote_map(data, info, i, new_len);
	rebuild_expand_map(data, info, i, new_len);
	new_arg = apply_expansion(arg, info, i, new_len);
	validate_malloc(data, new_arg, NULL);
	free(arg);
	return (new_arg);
}

char	*expand_variable(t_data *data, char *arg, int i, t_arg_info *info)
{
	char	*new_arg;
	char	*value;
	int		new_len;

	free(info->key);
	info->key = get_env_key(arg + i);
	validate_malloc(data, info->key, NULL);
	info->key_len = ft_strlen(info->key);
	free(info->value);
	value = get_env_value(data->env_list, info->key + 1);
	if (!value)
		info->value = ft_strdup("");
	else
		info->value = ft_strdup(value);
	validate_malloc(data, info->value, NULL);
	info->value_len = ft_strlen(value);
	new_len = ft_strlen(arg) - info->key_len + info->value_len;
	rebuild_quote_map(data, info, i, new_len);
	rebuild_expand_map(data, info, i, new_len);
	new_arg = apply_expansion(arg, info, i, new_len);
	validate_malloc(data, new_arg, NULL);
	free(arg);
	return (new_arg);
}

int	rebuild_quote_map(t_data *data, t_arg_info *info, int start, int new_len)
{
	int	*new_quote_map;
	int	old_len;
	int	quote_status;
	int	i;
	int	j;

	new_quote_map = ft_calloc(new_len, sizeof(int));
	validate_malloc(data, new_quote_map, NULL);
	old_len = new_len + info->key_len - info->value_len;
	i = 0;
	j = 0;
	while (i < start)
		new_quote_map[i++] = info->quote_map[j++];
	quote_status = 0;
	if (j < old_len)
		quote_status = info->quote_map[j];
	while (i < start + info->value_len)
		new_quote_map[i++] = quote_status;
	j += info->key_len;
	while (i < new_len && j < old_len)
		new_quote_map[i++] = info->quote_map[j++];
	free(info->quote_map);
	info->quote_map = new_quote_map;
	return (0);
}

int	rebuild_expand_map(t_data *data, t_arg_info *info, int start, int new_len)
{
	int	*new_expand_map;
	int	old_len;
	int	i;
	int	j;

	new_expand_map = ft_calloc(new_len, sizeof(int));
	validate_malloc(data, new_expand_map, NULL);
	old_len = new_len + info->key_len - info->value_len;
	i = 0;
	j = 0;
	if (info->expand_map)
		while (i < start && i < old_len)
			new_expand_map[i++] = info->expand_map[j++];
	i = start;
	while (i < start + info->value_len && i < new_len)
		new_expand_map[i++] = 1;
	if (info->expand_map)
	{
		j += info->key_len;
		while (i < new_len && j < old_len)
			new_expand_map[i++] = info->expand_map[j++];
	}
	free(info->expand_map);
	info->expand_map = new_expand_map;
	return (0);
}

char *apply_expansion(char *src, t_arg_info *info, int start, int len)
{
	char	*dest;

	dest = ft_calloc(len + 1, sizeof(char));
	if (!dest)
		return (NULL);
	if (len < 1)
		return (dest);
	ft_strlcpy(dest, src, start + 1);
	if (info->value)
		ft_strlcat(dest, info->value, len + 1);
	ft_strlcat(dest, src + start + info->key_len, len + 1);
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

char	*get_env_key(char *arg)
{
	char	*key;
	int		key_len;

	key_len = 0;
	if (arg[key_len] == '$')
		key_len++;
	while (arg[key_len] && (ft_isalnum(arg[key_len]) || arg[key_len] == '_'))
		key_len++;
	key = malloc(sizeof(char) * (key_len + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, arg, key_len + 1);
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
