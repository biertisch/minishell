/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 11:43:36 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/03 15:04:32 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//replace the argument with the list of matches

static void	filter_matches(t_list **head, char *wildcard)
{
	t_list	*curr;
	t_list	*prev;
	t_list	*tmp;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (!match_wildcard((char *)curr->content, wildcard))
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
		validate_malloc_wildcard(data, name, head, dir_stream);
		node = ft_lstnew(name);
		validate_malloc_wildcard(data, node, head, dir_stream);
		ft_lstadd_back(&head, node);
		dirent = readdir(dir_stream);
	}
	return (head);
}

int	expand_wildcard(t_data *data, char ***argv, int i)
{
	char	*dir_name;
	DIR		*dir_stream;
	t_list	*entries;

	dir_name = getcwd(NULL, 0);
	if (!dir_name)
		return (report_error("getcwd", SYSTEM_ERR));
	dir_stream = opendir(dir_name);
	free(dir_name);
	if (!dir_stream)
		return (report_error("opendir", SYSTEM_ERR));
	entries = get_entries(data, dir_stream);
	closedir(dir_stream);
	if (!entries)
		return (0);
	filter_matches(&entries, (*argv)[i]);
	if (!entries)
		return (0);
	//update_argv(data, argv, i, entries);
	ft_lstclear(&entries, free);
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
