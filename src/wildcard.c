/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 11:43:36 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 09:50:10 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	has_wildcard(const char *arg)
// {
// 	char	quote;

// 	if (!arg)
// 		return (0);
// 	quote = 0;
// 	while (*arg && *arg != '*')
// 	{
// 		update_quote_status(*arg, &quote);
// 		arg++;
// 	}
// 	return (!quote && *arg == '*');
// }

int	has_wildcard(char *arg, t_arg_info *info)
{
	int	i;

	if (!arg || !info)
		return (0);
	i = 0;
	while (arg[i] && arg[i] != '*')
		i++;
	return (arg[i] && arg[i] == '*' && !info->quote_map[i]);
}

int	expand_wildcard(t_data *data, t_tree *node)
{
	t_list	*entries;
	int		i;

	i = 0;
	while (node->argv[i])
	{
		if (has_wildcard(node->argv[i], node->argv_info + i))
		{
			if (expand_wildcard_arg(data, node->argv[i], &entries))
				return (-1);
			if (entries && rebuild_argv_wildcard(node, entries, i))
				validate_malloc_wildcard(data, NULL, entries, NULL);
			ft_lstclear(&entries, free);
		}
		i++;
	}
	return (0);
}


int	expand_wildcard_arg(t_data *data, char *arg, t_list **entries)
{
	char	*dir_name;
	DIR		*dir_stream;

	if (!arg)
		return (0);
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

void	filter_matches(t_list **head, char *pattern)
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

// char	*update_arg_wildcard(t_data *data, char *old_arg, t_list *entries)
// {
// 	char	*new_arg;

// 	if (!old_arg|| !entries)
// 		return (old_arg);
// 	new_arg = NULL;
// 	while (entries)
// 	{
// 		if (!new_arg)
// 			new_arg = ft_strdup(entries->content);
// 		else
// 			new_arg = append_entry(new_arg, entries->content);
// 		validate_malloc_wildcard(data, new_arg, entries, NULL); // revise this function, maybe last param is unnecessary)
// 		entries = entries->next;
// 	}
// 	free(old_arg);
// 	ft_lstclear(&entries, free);
// 	return (new_arg);
// }

// char	*append_entry(char *arg, char *entry)
// {
// 	char	*res;
// 	char	*tmp;

// 	tmp = ft_strjoin(" ", entry);
// 	if (!tmp)
// 	{
// 		free(arg);
// 		return (NULL);
// 	}
// 	res = ft_strjoin(arg, tmp);
// 	free(arg);
// 	free(tmp);
// 	return (res);
// }

char	*update_redir_wildcard(t_data *data, char *file, t_list *entry)
{
	char	*new_file;

	if (!file || !entry)
		return (file);
	new_file = ft_strdup(entry->content);
	validate_malloc_wildcard(data, new_file, entry, NULL);
	free(file);
	ft_lstclear(&entry, free);
	return (new_file);
}
