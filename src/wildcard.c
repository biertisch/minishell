/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 11:43:36 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/08 13:51:22 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*update_redir(t_data *data, char *file, t_list *entry)
{
	char	*new_file;

	new_file = ft_strdup(entry->content);
	validate_malloc_wildcard(data, new_file, entry, NULL);
	free(file);
	ft_lstclear(&entry, free);
	return (new_file);
}

static void	filter_matches(t_list **head, char *pattern)
{
	t_list	*curr;
	t_list	*prev;
	t_list	*tmp;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (!match_wildcard((char *)curr->content, pattern))
		{
			tmp = curr;
			curr = curr->next;
			if (prev)
				prev->next = curr;
			else
				*head = curr;
			ft_lstdelone(tmp, free);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

static t_list	*get_entries(t_data *data, DIR *dir_stream)
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
			validate_malloc_wildcard(data, name, head, NULL);
		}
		node = ft_lstnew(name);
		if (!node)
		{
			closedir(dir_stream);
			validate_malloc_wildcard(data, node, head, NULL);
		}
		ft_lstadd_back(&head, node);
		dirent = readdir(dir_stream);
	}
	return (head);
}

int	expand_wildcard(t_data *data, char *pattern, t_list **entries)
{
	char	*dir_name;
	DIR		*dir_stream;

	dir_name = getcwd(NULL, 0);
	if (!dir_name)
		return (system_error(data, "getcwd"));
	dir_stream = opendir(dir_name);
	free(dir_name);
	if (!dir_stream)
		return (system_error(data, "opendir"));
	*entries = get_entries(data, dir_stream);
	if (closedir(dir_stream))
		return (system_error(data, "closedir"));
	if (!entries)
		return (0);
	filter_matches(entries, pattern);
	return (0);
}

int	has_wildcard(const char *arg)
{
	while (*arg && *arg != '*')
		arg++;
	if (*arg == '*')
		return (1);
	return (0);
}
