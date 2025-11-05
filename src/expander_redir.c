/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:09:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/05 19:35:07 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_dollar_redir(t_data *data, t_redir *redir)
{
	char	*tmp;

	tmp = ft_strdup(redir->file);
	validate_malloc(data, tmp, NULL);
	expand_dollar(data, &redir->file, &redir->info);
	if (redir->info.key && !*(redir->info.value))
	{
		internal_error(data, INT_ERR_1, NULL, tmp);
		return (free(tmp), -1);
	}
	free(tmp);
	return (0);
}

int	expand_wildcard_redir(t_data *data, t_redir *redir)
{
	t_list	*entries;

	if (!has_wildcard(redir->file, &redir->info))
		return (0);
	if (expand_single_wildcard(data, redir->file, &entries))
		return (-1);
	if (entries)
	{
		if (entries->next)
		{
			ft_lstclear(&entries, free);
			return (internal_error(data, INT_ERR_1, NULL, redir->file));
		}
		redir->file = apply_redir_wildcard(data, redir->file, entries);
	}
	return (0);
}

char	*apply_redir_wildcard(t_data *data, char *file, t_list *entry)
{
	char	*new_file;

	if (!file || !entry)
		return (file);
	new_file = ft_strdup(entry->content);
	validate_malloc_wildcard(data, new_file, entry);
	free(file);
	ft_lstclear(&entry, free);
	return (new_file);
}
