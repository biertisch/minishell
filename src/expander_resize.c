/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_resize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:23:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/03 22:01:45 by beatde-a         ###   ########.fr       */
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
		if (!arg_disappears(argv[i], info + i))
			count += count_fields(argv[i], info + i);
		i++;
	}
	return (count);
}

char **resize_argv(t_data *data, char **argv, t_arg_info **info)
{
	char		**new_argv;
	int			new_argc;
	t_arg_info	*new_info;

	if (!argv || !info || !*info)
		return (argv);
	new_argc = get_expanded_argc(argv, *info);
	if (new_argc == get_argc(argv))
		return (argv);
	new_argv = ft_calloc(new_argc + 1, sizeof(char *));
	validate_malloc(data, new_argv, NULL);
	if (new_argc < 1)
		new_info = ft_calloc(1, sizeof(t_arg_info));
	else
		new_info = ft_calloc(new_argc, sizeof(t_arg_info));
	validate_malloc(data, new_info, NULL);
	rebuild_argv(data, new_argv, argv, *info, new_info);
	free_argv_info(info, get_argc(argv));
	*info = new_info;
	free_string_array(&argv);
	return (new_argv);
}

int	arg_disappears(char *arg, t_arg_info *info)
{
	return (!*arg && info->expand_map && info->expand_map[0] == 1
		&& !info->quote_map[0]);
}

int	is_ifs(char *arg, t_arg_info *info, int i)
{
	return (arg[i] && (arg[i] == ' ' || arg[i] == '\t' || arg[i] == '\n')
		&& info->expand_map && info->expand_map[i] == 1 && !info->quote_map[i]);
}

int	count_fields(char *arg, t_arg_info *info)
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

int	rebuild_argv(t_data *data, char **dest, char **src, t_arg_info *src_info, t_arg_info *dest_info)
{
	int	i;
	int	j;
	int	res;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (!arg_disappears(src[i], src_info + i) && has_ifs(src[i], src_info + i, 0 != -1))
		{
			res = split_on_ifs(dest + j, src[i], src_info + i, dest_info + j);
			if (res == -1)
				validate_malloc_unfinished_array(data, NULL, dest, j); // free dest_info on malloc failure
			j += res;
		}
		else if (!arg_disappears(src[i], src_info + i))
		{
			dest[j] = ft_strdup(src[i]);
			validate_malloc_unfinished_array(data, dest[j], dest, j); // free dest_info on malloc failure
			if (copy_arg_info(dest_info + j, src_info + i, 0, ft_strlen(src[i])))
				validate_malloc_unfinished_array(data, NULL, dest, j); // free dest_info on malloc failure
			j++;
		}
		i++;
	}
	return (0);
}

int	has_ifs(char *arg, t_arg_info *info, int i)
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

int	split_on_ifs(char **dest, char *src, t_arg_info *src_info, t_arg_info *dest_info)
{
	int start;
	int	end;
	int i;

	start = 0;
	i = 0;
	while (src[start])
	{
		while (is_ifs(src, src_info, start))
			start++;
		if (!src[start])
			break ;
		end = has_ifs(src, src_info, start);
		if (end == -1)
			end = ft_strlen(src);
		dest[i] = ft_substr(src, start, end);
		if (!dest[i] || copy_arg_info(dest_info + i, src_info, start, end))
			return (free_unfinished_string_array(dest, i), -1);
		i++;
		start = end;
	}
	dest[i] = NULL;
	return (i);
}

// t_arg_info	*rebuild_argv_info(t_data *data, char **new, char **old, t_arg_info *info)
// {
// 	t_arg_info 	*new_info;
// 	int			new_argc;
// 	int			old_argc;
// 	int			i;
// 	int			j;
// 	int			res;

// 	new_argc = get_argc(new);
// 	old_argc = get_argc(old);
// 	if (new_argc < 1)
// 		new_info = ft_calloc(1, sizeof(t_arg_info));
// 	else
// 		new_info = ft_calloc(new_argc, sizeof(t_arg_info));
// 	validate_malloc(data, new_info, NULL);
// 	i = 0;
// 	j = 0;
// 	while (i < new_argc && j < old_argc)
// 	{
// 		res = transfer_metadata(new[i], old[j], new_info + i, info + j);
// 		if (res == -1)
// 		{
// 			free_argv_info(&new_info, new_argc);
// 			validate_malloc_unfinished_array(data, NULL, new, new_argc);
// 		}
// 		i += res;
// 		if (res <= 1)
// 			j++;
// 	}
// }

// int	transfer_metadata(char *new, char *old, t_arg_info *dest, t_arg_info *src)
// {
// 	int	splits;

// 	if (ft_strcmp(new, old) && arg_disappears(old, src))
// 		return (0);
// 	splits = count_fields(old, src);
// 	if (splits > 1 && copy_split_arg_info(new, old, dest, src))
// 		return (-1);
// 	else if (splits == 1 && copy_arg_info(dest, src, ft_strlen(old)))
// 		return (-1);
// 	return (splits);
// }

int	copy_arg_info(t_arg_info *dest, t_arg_info *src, int start, int end)
{
	int	len;

	len = end - start;
	if (len <= 0)
		return (0);
	dest->quote_map = ft_calloc(len, sizeof(int));
	if (!dest->quote_map)
		return (-1);
	ft_memcpy(dest->quote_map, src->quote_map, len * sizeof(int));
	dest->expand_map = ft_calloc(len, sizeof(int));
	if (!dest->expand_map)
		return (-1);
	ft_memcpy(dest->expand_map, src->expand_map, len * sizeof(int));
	return (0);
}
