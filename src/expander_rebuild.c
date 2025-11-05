/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_rebuild.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:23:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 22:14:03 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**build_expanded_argv(t_data *data, char **argv, t_metadata **info)
{
	char		**new_argv;
	int			new_argc;
	int			old_argc;
	t_metadata	*new_info;

	if (!argv || !info || !*info)
		return (argv);
	new_argc = get_expanded_argc(argv, *info);
	old_argc = get_argc(argv);
	if (new_argc == old_argc)
		return (argv);
	new_argv = ft_calloc(new_argc + 1, sizeof(char *));
	validate_malloc(data, new_argv, NULL);
	if (new_argc < 1)
		new_info = ft_calloc(1, sizeof(t_metadata));
	else
		new_info = ft_calloc(new_argc, sizeof(t_metadata));
	validate_malloc(data, new_info, NULL);
	if (rebuild_argv(new_argv, argv, *info, new_info))
		handle_malloc_failure_expansion(data, new_argv, new_info, new_argc);
	free_metadata(info, old_argc);
	*info = new_info;
	free_string_array(&argv);
	return (new_argv);
}

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

int	get_expanded_argc(char **argv, t_metadata *info)
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

int	rebuild_argv(char **dest, char **src, t_metadata *s_info,
	t_metadata *d_info)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (has_ifs(src[i], s_info + i, 0) != -1)
		{
			if (split_on_ifs(dest + j, src[i], s_info + i, d_info + j) == -1)
				return (-1);
		}
		else if (!arg_disappears(src[i], s_info + i))
		{
			dest[j] = ft_strdup(src[i]);
			if (!dest[j]
				|| copy_arg_info(d_info + j, s_info + i, 0, ft_strlen(src[i])))
				return (-1);
		}
		j += count_fields(src[i], s_info + i);
		i++;
	}
	return (0);
}

int	copy_arg_info(t_metadata *dest, t_metadata *src, int start, int end)
{
	int	len;

	len = end - start;
	if (len <= 0)
		return (0);
	dest->quote_map = ft_calloc(len, sizeof(int));
	dest->expand_map = ft_calloc(len, sizeof(int));
	if (!dest->quote_map || !dest->expand_map)
		return (-1);
	if (src->key)
		dest->key = ft_strdup(src->key);
	if (src->value)
		dest->value = ft_strdup(src->value);
	if ((src->key && !dest->key) || (src->value && !dest->value))
		return (-1);
	if (src->quote_map)
		ft_memcpy(dest->quote_map, src->quote_map + start, len * sizeof(int));
	if (src->expand_map)
		ft_memcpy(dest->expand_map, src->expand_map + start, len * sizeof(int));
	dest->key_len = src->key_len;
	dest->value_len = src->value_len;
	dest->total_len = src->total_len;
	return (0);
}
