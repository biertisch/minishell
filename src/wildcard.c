/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 11:43:36 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/02 16:36:41 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//match each entry against the pattern
//replace the argument with the list of matches
static int	filter_matches(t_data *data, t_list **head, char *wildcard)
{
	t_list	*curr;
	t_list	*prev;
	t_list	*tmp;

	prev = *head;
	curr = (*head)->next;

	while (curr)
	{
		if (!match_wildcard((*head)->content, wildcard))
		{
			tmp = curr;
			prev->next = curr->next;
			curr = curr->next;
			prev 
			ft_lstdelone(tmp, free);
		}
	}

	
	while (*head && !match_wildcard((*head)->content, wildcard))
	{
		tmp = *head;
		*head = (*head)->next;
		ft_lstdelone(tmp, free);
	}
	if (*head)
		trav = (*head)->next;
	while (trav)
	{
		
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
	if (!dir_stream)
		return (report_error("opendir", SYSTEM_ERR));
	free(dir_name);
	entries = get_entries(data, dir_stream);
	if (closedir(dir_stream))
	{
		ft_lstclear(&entries, free);
		return (report_error("closedir", SYSTEM_ERR));	
	}
	if (!entries)
		return (0);
	if	(!filter_matches(data, &entries, (*argv)[i]))
		return (0);
	update_argv(data, argv, i, entries);
	return (0);
}

int	has_wildcard(const char *arg)
{
	while (*arg)
	{
		if (*arg == '*')
			return (1);
		arg++;	
	}
	return (0);
}
