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

int	is_command_token(t_token_type token_type)
{
	return (token_type == WORD || token_type == FD
		|| is_redir_token(token_type));
}

//counts all WORDs as argv, except for
//the one immediately after a redirection operator
static int	count_argv(t_token *token)
{
	int	count;

	count = 0;
	while (token && is_command_token(token->type))
	{
		if (is_redir_token(token->type) && token->next
			&& token->next->type == WORD)
			token = token->next;
		else if (token->type == WORD)
			count++;
		token = token->next;
	}
	return (count);
}

static char	**allocate_argv(t_data *data, t_token **token, t_tree *node)
{
	char	**argv;
	int		count;

	count = count_argv(*token);
	if (count <= 0)
		return (NULL);
	argv = malloc(sizeof(char *) * (count + 1));
	validate_malloc_tree(data, argv, node, NULL);
	return (argv);
}

//counts all WORDS as part of argv except for
//the WORD immediately after redirection operator
//checks for empty commands (with no argv and no redir)
int	get_command_data(t_data *data, t_token **token, t_tree *node)
{
	int	i;

	node->argv = allocate_argv(data, token, node);
	i = 0;
	while (*token && is_command_token((*token)->type))
	{
		if (is_redir_token((*token)->type) || (*token)->type == FD)
			node->redir = get_redir(data, token);
		else if ((*token)->type == WORD)
		{
			node->argv[i] = ft_strdup((*token)->value);
			validate_malloc_tree(data, node->argv[i], node, NULL);
			i++;
			*token = (*token)->next;
		}
	}
	if (node->argv)
		node->argv[i] = NULL;
	if (!node->argv && !node->redir)
		return (report_error("invalid command", SYNTAX_ERR));
	return (0);
}
