/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:37:28 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/08 17:09:03 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_tree	*empty_subshell(t_token **token, t_tree *node)
{
	if (*token && (*token)->type == RPAREN)
		*token = (*token)->next;
	free_parser_node(&node);
	return (NULL);
}

t_tree	*invalid_sequence(t_data *data, t_token *token, t_tree *node)
{
	if (!token)
		syntax_error(data, ERR_2, "newline");
	else
		syntax_error(data, ERR_2, token->value);
	free_parser_tree(&node);
	return (NULL);
}
