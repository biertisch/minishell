/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 10:22:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/06 14:06:06 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_redir(t_data *data, t_token **token, t_tree *node)
{
	t_redir			*last;
	t_redir			*curr;

	last = node->redir;
	while (*token && (is_redir_token((*token)->type) || (*token)->type == FD))
	{
		curr = parse_single_redir(data, token, node->redir);
		if (!curr)
			return (-1);
		if (!node->redir)
			node->redir = curr;
		else
			last->next = curr;
		last = curr;
	}
	return (0);
}

t_redir	*parse_single_redir(t_data *data, t_token **token, t_redir *head)
{
	t_redir			*redir;
	t_token_type	type;
	int				fd;

	fd = -1;
	if ((*token)->type == FD)
	{
		fd = ft_atoi((*token)->value);
		*token = (*token)->next;
	}
	type = (*token)->type;
	*token = (*token)->next;
	if (!*token)
		return (syntax_error(data, SYN_ERR_5, "newline"), NULL);
	if ((*token)->type != WORD)
		return (syntax_error(data, SYN_ERR_5, (*token)->value), NULL);
	redir = create_redir(type, fd, (*token)->value);
	if (!redir)
	{
		system_error(strerror(errno), "malloc");
		free_redir(head);
		error_exit(data, NULL);
	}
	*token = (*token)->next;
	return (redir);
}

t_redir	*create_redir(t_token_type type, int fd, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->fd = fd;
	redir->file = ft_strdup(file);
	if (!redir->file)
		return (free_redir(redir), NULL);
	redir->heredoc_input = NULL;
	ft_bzero(&redir->info, sizeof(t_metadata));
	redir->next = NULL;
	return (redir);
}
