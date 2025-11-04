/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_argv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 11:30:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 11:06:15 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	rebuild_argv_wildcard(t_tree *node, t_list *entries, int star_index)
{
	char		**new_argv;
	t_arg_info	*new_info;
	int			count;
	int			i;

	if (!node || !node->argv || !node->argv_info || !entries)
		return (0);
	count = get_argc(node->argv) + ft_lstsize(entries) - 1;
	new_argv = ft_calloc(count + 1, sizeof(char *));
	if (!new_argv)
		return (-1);
	new_info = ft_calloc(count, sizeof(t_arg_info));
	if (!new_info)
		return (free(new_argv), -1);
	i = copy_before_star(node, new_argv, new_info, star_index);
	if (i < 0)
		return (free_argv_info(&new_info, get_argc(new_argv)), free_string_array(&new_argv), -1);
	i = copy_entries(new_argv, new_info, entries, i);
	if (i < 0)
		return (free_argv_info(&new_info, get_argc(new_argv)), free_string_array(&new_argv), -1);
	if (copy_after_star(node, new_argv + i, new_info + i, star_index + 1))
		return (free_argv_info(&new_info, get_argc(new_argv)), free_string_array(&new_argv), -1);
	free_argv_info(&node->argv_info, get_argc(node->argv));
	free_string_array(&node->argv);
	node->argv_info = new_info;
	node->argv = new_argv;
	return (0);
}

// int	count_expanded_argv(char **argv, t_list *head)
// {
// 	int		old_count;
// 	int		add_count;

// 	old_count = 0;
// 	while (argv[old_count])
// 		old_count++;
// 	add_count = 0;
// 	while (head)
// 	{
// 		add_count++;
// 		head = head->next;
// 	}
// 	return (old_count - 1 + add_count);
// }

int	copy_entries(char **new_argv, t_arg_info *new_info, t_list *entries, int start)
{
	while (entries)
	{
		new_argv[start] = ft_strdup((char *)entries->content);
		if (!new_argv[start])
			return (-1);
		new_info[start].quote_map = ft_calloc(ft_strlen(new_argv[start]), sizeof(int));
		if (!new_info[start].quote_map)
			return (-1);
		entries = entries->next;
		start++;
	}
	return (start);
}

int	copy_before_star(t_tree *node, char **new_argv, t_arg_info *new_info, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		new_argv[i] = ft_strdup(node->argv[i]);
		if (!new_argv[i])
			return (-1);
		if (copy_arg_info(&new_info[i], &node->argv_info[i], 0, ft_strlen(node->argv[i])))
			return (-1);
		i++;
	}
	return (i);
}

int	copy_after_star(t_tree *node, char **new_argv, t_arg_info *new_info, int start)
{
	int	i;

	i = 0;
	while (node->argv[start])
	{
		new_argv[i] = ft_strdup(node->argv[start]);
		if (!new_argv[i])
			return (-1);
		if (copy_arg_info(&new_info[i], &node->argv_info[start], 0, ft_strlen(node->argv[start])))
			return (-1);
		i++;
		start++;
	}
	new_argv[i] = NULL;
	return (0);
}
