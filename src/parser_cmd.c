/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:10:37 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/26 11:10:37 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	*create_command(char **argv, t_redir *redirs)
{
	t_cmd	*command;

	command = malloc(sizeof(t_cmd));
	if (!command)
		return (NULL);
	command->argv = argv;
	command->redirs = redirs;
	return (command);
}

static t_redir	*create_redir(t_token_type type, int fd, char *file, t_redir *next)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	if (fd == -1)
	{
		if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
			fd = 0;
		else
			fd = 1;
	}
	redir->type = type;
	redir->fd = fd;
	redir->file = file;
	redir->next = next;
	return (redir);
}

t_redir	*get_redirs(t_data *data, t_token **token)
{
	t_redir			*redir;
	t_token_type	type;
	int				fd;
	char			*file;

	if (!*token || (!is_redir((*token)->type) && (*token)->type != TOKEN_FD))
		return (NULL);
	fd = -1;
	if ((*token)->type == TOKEN_FD)
	{
		fd = ft_atoi((*token)->value);
		*token = (*token)->next;
	}
	type = (*token)->type;
	*token = (*token)->next;
	if (!*token || (*token)->type != TOKEN_WORD)
		return (report_error("missing filename", SYNTAX_ERR), NULL);
	file = ft_strdup((*token)->value);
	validate_malloc(data, file);
	*token = (*token)->next;
	redir = create_redir(type, fd, file, get_redirs(data, token));
	validate_malloc(data, redir);
	return (redir);
}

static char **allocate_argv(t_data *data, t_token **token)
{
	char	**argv;
	int		count;
	
	count = count_argv(*token);
	if (count <= 0)
		return (NULL);
	argv = malloc(sizeof(char *) * (count + 1));
	validate_malloc(data, argv);
	return (argv);
}

int	get_command_data(t_data *data, t_token **token, char ***argv, t_redir **redirs)
{
	int		i;

	*argv = allocate_argv(data, token);
	i = 0;
	while (*token && is_command_token((*token)->type))
	{
		if (is_redir((*token)->type) || (*token)->type == TOKEN_FD)
			*redirs = get_redirs(data, token);
		else if ((*token)->type == TOKEN_WORD)
		{
			if (*argv)
			{
				(*argv)[i] = ft_strdup((*token)->value);
				validate_malloc(data, (*argv)[i]);
				i++;
			}
			*token = (*token)->next;
		}
	}
	if (*argv)
		(*argv)[i] = NULL;
	if (!*argv && !*redirs)
		return (report_error("empty command", SYNTAX_ERR));
	return (0);
}
