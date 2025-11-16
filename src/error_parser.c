/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 12:33:22 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/16 15:36:13 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	missing_quote(t_data *data, char quote)
{
	if (quote == '\'')
		return (syntax_error(data, SYN_ERR_0, "'"));
	return (syntax_error(data, SYN_ERR_0, "\""));
}

int	check_unsupported_syntax(t_data *data, char *input, char quote)
{
	if (!quote && *input == ';')
		return (syntax_error(data, SYN_ERR_1, NULL));
	if (quote != '\'' && !ft_strncmp(input, "$((", 3))
		return (syntax_error(data, SYN_ERR_3, NULL));
	if (quote != '\'' && !ft_strncmp(input, "${", 2))
		return (syntax_error(data, SYN_ERR_9, NULL));
	if (quote != '\'' && !ft_strncmp(input, "$(", 2))
		return (syntax_error(data, SYN_ERR_4, NULL));
	if (!quote && is_arithmetic_op(input))
		return (syntax_error(data, SYN_ERR_2, NULL));
	return (0);
}

void	validate_malloc_parser(t_data *data, void *ptr, t_tree *left,
	t_tree *right)
{
	if (ptr)
		return ;
	system_error(strerror(errno), "malloc");
	free_parser_tree(&left);
	free_parser_tree(&right);
	error_exit(data, NULL);
}
