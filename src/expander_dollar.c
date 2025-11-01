/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:43:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/01 21:25:04 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dollar_expansion(char quote, char *arg)
{
	return (quote != '\'' && arg[0] == '$' && arg[1]
		&& (ft_isalpha(arg[1]) || arg[1] == '_' || arg[1] == '?'));
}

int	has_dollar(char *arg)
{
	int		i;
	char	quote;

	if (!arg)
		return (0);
	quote = 0;
	i = 0;
	while (arg[i])
	{
		update_quote_status(arg[i], &quote);
		if (is_dollar_expansion(quote, arg + i))
			return (1);
		i++;
	}
	return (0);
}

int	expand_dollar(t_data *data, char **arg, t_arg_info *arg_info)
{
	int		i;
	char	quote;

	if (!arg || !*arg || !arg_info)
		return (0);
	quote = 0;
	i = 0;
	while ((*arg)[i])
	{
		update_quote_status(arg[i], &quote);
		if (is_dollar_expansion(quote, *arg + i))
		{
			arg_info->expanded = 1;
			if (quote == '"')
				arg_info->in_quotes = 1;
			if ((*arg)[i + 1] == '?')
				*arg = expand_exit_status(data, *arg, i);
			else
				*arg = expand_variable(data, *arg, i);
		}
		i++;
	}
	return (0);
}

char	*expand_exit_status(t_data *data, char *old_arg, int i,
	t_arg_info *arg_info)
{
	char	*value;
	char	*new_arg;

	value = ft_itoa(data->exit_status);
	validate_malloc(data, value, NULL);
	new_arg = update_arg(old_arg, i, "?", value);
	validate_malloc(data, new_arg, value);
	free(value);
	arg_info->start = i;
	arg_info->end = i + ft_strlen(value);
	return (new_arg);
}

char	*expand_variable(t_data *data, char *old_arg, int i,
	t_arg_info *arg_info)
{
	char	*key;
	char	*value;
	char	default_value[1];
	char	*new_arg;

	default_value[0] = '\0';
	key = get_env_key(old_arg[i + 1]);
	validate_malloc(data, key, NULL);
	value = get_env_value(data->env_list, key);
	if (!value)
		value  = default_value;
	new_arg = update_arg(old_arg, i, key, value);
	validate_malloc(data, new_arg, key);
	free(key);
	arg_info->start = i;
	arg_info->end = i + ft_strlen(value);
	return (new_arg);
}


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
	while (arg[key_len] && (ft_isalnum(arg[key_len]) || arg[key_len] == '_'))
		key_len++;
	key = malloc(sizeof(char) * (key_len + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, arg, key_len + 1);
	return (key);
}

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
