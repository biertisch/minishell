/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:37:28 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/29 11:08:07 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	incomplete_subshell(t_data *data, t_tree **node)
{
	free_parser_tree(data, node);
	return (INCOMPLETE_PAREN);
}

int	empty_subshell(t_data *data, t_token **token, t_tree *node, int res)
{
	if (*token && (*token)->type == RPAREN)
		*token = (*token)->next;
	free_parser_tree(data, &node);
	return (res);
}

int	invalid_sequence(t_data *data, t_token *token, t_tree *node)
{
	free_parser_tree(data, &node);
	return (syntax_error(data, ERR_1, token->value));
}
