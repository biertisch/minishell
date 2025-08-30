/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 10:22:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/30 10:22:02 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_redir_token(t_token_type token_type)
{
	return (token_type == REDIR_IN || token_type == REDIR_OUT
		|| token_type == APPEND || token_type == HEREDOC);
}

//if fd is not explict, sets 0 as default fd for '<' and '<<'
//and 1 as default fd for '>' and '>>'
static t_redir	*create_redir(t_token_type type, int fd, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	if (fd == -1 && (type == REDIR_IN || type == HEREDOC))
		fd = 0;
	else if (fd == -1 && (type == REDIR_OUT || type == APPEND))
		fd = 1;
	redir->type = type;
	redir->fd = fd;
	redir->file = ft_strdup(file);
	if (!redir->file)
		return (free_redir(redir), NULL);
	redir->next = NULL;
	return (redir);
}

static t_redir	*parse_single_redir(t_data *data, t_token **token,
	t_redir *head)
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
	if (!*token || (*token)->type != WORD)
		return (NULL);
	redir = create_redir(type, fd, (*token)->value);
	if (!redir)
	{
		report_error("malloc", SYSTEM_ERR);
		free_redir(head);
		error_exit(data);
	}
	*token = (*token)->next;
	return (redir);
}

//checks for missing filename after redirection operator
t_redir	*get_redir(t_data *data, t_token **token)
{
	t_redir			*head;
	t_redir			*last;
	t_redir			*curr;

	head = NULL;
	last = NULL;
	while (*token && (is_redir_token((*token)->type) || (*token)->type == FD))
	{
		curr = parse_single_redir(data, token, head);
		if (!curr)
		{
			free_redir(head);
			return (report_error("missing filename", SYNTAX_ERR), NULL);
		}
		if (!head)
			head = curr;
		else
			last->next = curr;
		last = curr;
	}
	return (head);
}
