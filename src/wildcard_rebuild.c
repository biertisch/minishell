/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_rebuild.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 11:30:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 22:13:44 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	build_wildcard_argv(t_tree *node, t_list *entries, int star_index)
{
	char		**new_argv;
	t_metadata	*new_info;
	int			count;
	int			i;

	if (!node || !node->argv || !node->argv_info || !entries)
		return (0);
	count = get_argc(node->argv) + ft_lstsize(entries) - 1;
	new_argv = ft_calloc(count + 1, sizeof(char *));
	new_info = ft_calloc(count, sizeof(t_metadata));
	if (!new_argv || !new_info)
		return (free(new_argv), free(new_info), -1);
	i = copy_before_star(node, new_argv, new_info, star_index);
	if (i < 0)
		return (handle_wildcard_rebuild_failure(new_argv, new_info, count));
	i = copy_entries(new_argv, new_info, entries, i);
	if (i < 0)
		return (handle_wildcard_rebuild_failure(new_argv, new_info, count));
	if (copy_after_star(node, new_argv + i, new_info + i, star_index + 1))
		return (handle_wildcard_rebuild_failure(new_argv, new_info, count));
	free_metadata(&node->argv_info, get_argc(node->argv));
	free_string_array(&node->argv);
	node->argv_info = new_info;
	node->argv = new_argv;
	return (0);
}

int	copy_entries(char **argv, t_metadata *info, t_list *entries, int start)
{
	int	len;

	while (entries)
	{
		argv[start] = ft_strdup((char *)entries->content);
		if (!argv[start])
			return (-1);
		len = ft_strlen(argv[start]);
		info[start].quote_map = ft_calloc(len, sizeof(int));
		if (!info[start].quote_map)
			return (-1);
		entries = entries->next;
		start++;
	}
	return (start);
}

int	copy_before_star(t_tree *node, char **argv, t_metadata *info, int size)
{
	int	i;
	int	len;

	i = 0;
	while (i < size)
	{
		argv[i] = ft_strdup(node->argv[i]);
		if (!argv[i])
			return (-1);
		len = ft_strlen(node->argv[i]);
		if (copy_arg_info(&info[i], &node->argv_info[i], 0, len))
			return (-1);
		i++;
	}
	return (i);
}

int	copy_after_star(t_tree *node, char **argv, t_metadata *info, int start)
{
	int	i;
	int	len;

	i = 0;
	while (node->argv[start])
	{
		argv[i] = ft_strdup(node->argv[start]);
		if (!argv[i])
			return (-1);
		len = ft_strlen(node->argv[start]);
		if (copy_arg_info(&info[i], &node->argv_info[start], 0, len))
			return (-1);
		i++;
		start++;
	}
	argv[i] = NULL;
	return (0);
}
