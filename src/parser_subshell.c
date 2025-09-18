/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:37:28 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/18 10:19:38 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
