/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 12:33:22 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/05 12:40:36 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unsupported_syntax(t_data *data, char *input)
{
	if (*input == ';')
		return (syntax_error(data, ERR_10, NULL));
	if (!ft_strncmp(input, "$((", 3))
		return (syntax_error(data, ERR_12, NULL));
	if (!ft_strncmp(input, "$(", 2))
		return (syntax_error(data, ERR_13, NULL));
	if (is_arithmetic_op(input))
		return (syntax_error(data, ERR_9, NULL));
	return (0);
}
