/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:24:26 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 16:24:57 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_name(char *s)
{
	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	while (*s)
	{
		if (!ft_isalnum(*s) && *s != '_')
			return (0);
		s++;
	}
	return (1);
}

int	is_new_var(char *arg)
{
	if (!arg || (!ft_isalpha(*arg) && *arg != '_'))
		return (0);
	while (*arg && (ft_isalnum(*arg) || *arg == '_'))
		arg++;
	return (*arg == '=');
}
