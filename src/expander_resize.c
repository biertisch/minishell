/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_resize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:23:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/03 19:30:56 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_argc(char **argv)
{
	int	i;

	if (!argv || !*argv)
		return (0);
	i = 0;
	while (argv[i])
		i++;
	return (i);
}

int	get_expanded_argc(char **argv, t_arg_info *info)
{
	int	count;
	int	i;

	if (!argv || !*argv || !info)
		return (0);
	count = 0;
	i = 0;
	while (argv[i])
	{
		if (!arg_disappears(argv[i], info[i]))
			count += count_ifs_fields(argv[i], info[i]);
		i++;
	}
	return (count);
}

char **resize_argv(t_data *data, char **argv, t_arg_info **info)
{
	char		**new_argv;
	int			new_argc;
	// t_arg_info	*new_info;

	if (!argv || !info || !*info)
		return (argv);
	new_argc = get_expanded_argc(argv, *info);
	if (new_argc == get_argc(argv))
		return (argv);
	new_argv = ft_calloc(new_argc + 1, sizeof(char *));
	validate_malloc(data, new_argv, NULL);
	rebuild_argv(data, new_argv, argv, *info);
	//update argv_info
	free_string_array(&argv);
	return (new_argv);
}

int	arg_disappears(char *arg, t_arg_info info)
{
	return (!*arg && info.expand_map && info.expand_map[0] == 1
		&& !info.quote_map[0]);
}

int	is_ifs(char *arg, t_arg_info info, int i)
{
	return (arg[i] && (arg[i] == ' ' || arg[i] == '\t' || arg[i] == '\n')
		&& info.expand_map && info.expand_map[i] == 1 && !info.quote_map[i]);
}

int	count_ifs_fields(char *arg, t_arg_info info)
{
	int	i;
	int	count;
	int	in_field;

	if (!arg)
		return (0);
	count = 0;
	in_field = 0;
	i = 0;
	while (arg[i])
	{
		if (!is_ifs(arg, info, i) && !in_field)
		{
			count++;
			in_field = 1;
		}
		else if (is_ifs(arg, info, i))
			in_field = 0;
		i++;
	}
	if (count == 0 && !arg_disappears(arg, info))
		count++;
	return (count);
}

int	rebuild_argv(t_data *data, char **dest, char **src, t_arg_info *info)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (!arg_disappears(src[i], info[i]) && has_ifs(src[i], info[i], 0 != -1))
		{
			j += split_on_ifs(dest + j, src[i], info[i]);
			if (j == -1)
				validate_malloc_unfinished_array(data, NULL, dest, j);
		}
		else if (!arg_disappears(src[i], info[i]))
		{
			dest[j] = ft_strdup(src[i]);
			validate_malloc_unfinished_array(data, dest[j], dest, j);
			j++;
		}
		i++;
	}
	return (0);
}

int	has_ifs(char *arg, t_arg_info info, int i)
{
	if (!arg)
		return (-1);
	while (arg[i])
	{
		if (is_ifs(arg, info, i))
			return (i);
		i++;
	}
	return (-1);
}

int	split_on_ifs(char **dest, char *src, t_arg_info info)
{
	int i;
	int j;
	int	end;

	i = 0;
	j = 0;
	while (src[i])
	{
		while (is_ifs(src, info, i))
			i++;
		if (!src[i])
			break ;
		end = has_ifs(src, info, i);
		if (end == -1)
			end = ft_strlen(src);
		dest[j] = ft_substr(src, i, end);
		if (!dest[j])
			return (free_unfinished_string_array(dest, j), -1);
		j++;
		i = end;
	}
	dest[j] = NULL;
	return (j);
}
