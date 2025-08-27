/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 11:10:37 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/26 11:10:37 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_redir	*create_redir(t_token_type type, char *file, t_redir *next)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	redir->next = next;
	return (redir);
}

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

t_redir	*get_command_redirs(t_data *data, t_token **token)
{
	t_redir			*redir;
	t_token_type	type;
	char			*file;

	if (!*token || !is_redir((*token)->type))
		return (NULL);
	type = (*token)->type;
	*token = (*token)->next;
	if (!*token || (*token)->type != TOKEN_WORD)
		return (report_error("missing filename", SYNTAX_ERR), NULL);
	file = ft_strdup((*token)->value);
	validate_malloc(data, file);
	*token = (*token)->next;
	redir = create_redir(type, file, get_command_redirs(data, token));
	validate_malloc(data, redir);
	return (redir);
}

static int	count_words(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type == TOKEN_WORD)
	{
		count++;
		token = token->next;
	}
	return (count);
}

char	**get_command_argv(t_data *data, t_token **token)
{
	char	**argv;
	int		count;
	int		i;

	count = count_words(*token);
	if (!count)
		return (NULL);
	argv = malloc(sizeof(char *) * (count + 1));
	validate_malloc(data, argv);
	i = 0;
	while (*token && (*token)->type == TOKEN_WORD)
	{
		argv[i] = ft_strdup((*token)->value);
		validate_malloc(data, argv[i]);
		*token = (*token)->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}
