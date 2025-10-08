/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:43:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/08 15:50:50 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	copy_to_array(char **dest, char **src, int size)
{
	int	i;

	i = 0;
	while (i < size && src[i])
	{
		dest[i] = ft_strdup(src[i]);
		if (!dest[i])
		{
			i--;
			while (i >= 0)
			{
				free(dest[i]);
				i--;
			}
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	get_argv_count(char **old_argv, char **add_argv, int *add_count)
{
	int	old_count;

	old_count = 0;
	while (old_argv[old_count])
		old_count++;
	*add_count = 0;
	while (add_argv[*add_count])
		(*add_count)++;
	return (old_count + *add_count - 1);
}

static int	build_argv(char **new_argv, char **old_argv, char **to_add, int i)
{
	int		add_count;
	int		count;

	count = get_argv_count(old_argv, to_add, &add_count);
	if (copy_to_array(new_argv, old_argv, i))
		return (-1);
	if (copy_to_array(new_argv + i, to_add, add_count))
		return (-1);
	if (i + add_count < count)
		if (copy_to_array(new_argv + i + add_count, old_argv + i + 1, count))
			return (-1);
	return (0);
}

static char	**update_all_arg(char **argv, int i, char *value)
{
	char	**new_argv;
	char	**to_add;
	int		add_count;
	int		count;

	to_add = ft_split(value, ' ');
	if (!to_add)
		return (NULL);
	count = get_argv_count(argv, to_add, &add_count);
	new_argv = malloc(sizeof(char *) * (count + 1));
	if (!new_argv)
	{
		free_string_array(&to_add);
		return (NULL);
	}
	new_argv[count] = NULL;
	if (build_argv(new_argv, argv, to_add, i))
	{
		free(new_argv);
		free_string_array(&to_add);
		return (NULL);
	}
	free_string_array(&argv);
	free_string_array(&to_add);
	return (new_argv);
}

static char	*update_single_arg(char *arg, int i, char *key, char *value)
{
	char	*new_arg;
	int		new_len;

	new_len = ft_strlen(arg) - ft_strlen(key) - 1 + ft_strlen(value);
	new_arg = malloc(sizeof(char) * (new_len + 1));
	if (!new_arg)
		return (NULL);
	ft_strlcpy(new_arg, arg, i + 1);
	ft_strlcat(new_arg, value, new_len + 1);
	ft_strlcat(new_arg, arg + i + 1 + ft_strlen(key), new_len + 1);
	free(arg);
	return (new_arg);
}

static char	*get_env_key(char *arg)
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

static void	expand_exit_status(t_data *data, char **arg, int i)
{
	char	*value;

	value = ft_itoa(data->exit_status);
	validate_malloc(data, value, NULL);
	*arg = update_single_arg(*arg, i, "?", value);
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
	if (!value)
		value = "";
	if (arg[index][0] == '"' || !ft_strchr(value, ' '))
	{
		arg[index] = update_single_arg(arg[index], i, key, value);
		validate_malloc(data, arg[index], key);
	}
	else
	{
		arg = update_all_arg(arg, index, value);
		validate_malloc(data, arg, key);
	}
	free(key);
	return (arg);
}

char	**expand_dollar(t_data *data, char **arg, int idx)
{
	int	i;

	i = 0;
	while (arg[idx][i])
	{
		if (arg[idx][i] == '$' && arg[idx][i + 1] && arg[idx][i + 1] != ' ')
		{
			if (arg[idx][i + 1] == '?')
				expand_exit_status(data, &arg[idx], i);
			else
				arg = expand_variable(data, arg, idx, i);
			i = -1;
		}
		i++;
	}
	return (arg);
}
