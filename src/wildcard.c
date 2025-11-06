/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 11:43:36 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/06 14:05:45 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_wildcard(t_data *data, t_tree *node)
{
	t_list	*entries;
	int		i;

	i = 0;
	while (node->argv[i])
	{
		if (has_wildcard(node->argv[i], node->argv_info + i))
		{
			if (expand_single_wildcard(data, node->argv[i], &entries))
				return (-1);
			if (entries && build_wildcard_argv(node, entries, i))
				validate_malloc_wildcard(data, NULL, entries);
			ft_lstclear(&entries, free);
		}
		i++;
	}
	return (0);
}

int	has_wildcard(char *arg, t_metadata *info)
{
	int	i;

	if (!arg || !info)
		return (0);
	i = 0;
	while (arg[i] && arg[i] != '*')
		i++;
	return (arg[i] && arg[i] == '*' && !info->quote_map[i]);
}

int	expand_single_wildcard(t_data *data, char *arg, t_list **entries)
{
	char	*dir_name;
	DIR		*dir_stream;

	if (!arg)
		return (0);
	dir_name = getcwd(NULL, 0);
	if (!dir_name)
		return (system_error(strerror(errno), "getcwd"));
	dir_stream = opendir(dir_name);
	free(dir_name);
	if (!dir_stream)
		return (system_error(strerror(errno), "opendir"));
	*entries = get_entries(data, dir_stream);
	if (closedir(dir_stream))
		return (system_error(strerror(errno), "closedir"));
	if (!entries)
		return (0);
	filter_matches(entries, arg);
	return (0);
}

t_list	*get_entries(t_data *data, DIR *dir_stream)
{
	struct dirent	*dirent;
	t_list			*head;
	t_list			*node;
	char			*name;

	head = NULL;
	dirent = readdir(dir_stream);
	while (dirent)
	{
		name = ft_strdup(dirent->d_name);
		if (!name)
		{
			closedir(dir_stream);
			validate_malloc_wildcard(data, name, head);
		}
		node = ft_lstnew(name);
		if (!node)
		{
			closedir(dir_stream);
			validate_malloc_wildcard(data, node, head);
		}
		ft_lstadd_back(&head, node);
		dirent = readdir(dir_stream);
	}
	return (head);
}
