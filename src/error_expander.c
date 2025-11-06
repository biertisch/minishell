/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:04:39 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/06 14:02:40 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_malloc_failure_expansion(t_data *data, char **argv, t_metadata *info,
	int argc)
{
	free_metadata(&info, argc);
	free_string_array(&argv);
	validate_malloc(data, NULL, NULL);
	return (-1);
}

int	handle_wildcard_rebuild_failure(char **argv, t_metadata *info, int argc)
{
	free_metadata(&info, argc);
	free_string_array(&argv);
	return (-1);
}

void	validate_malloc_wildcard(t_data *data, void *ptr, t_list *node)
{
	if (!ptr)
	{
		system_error(strerror(errno), "malloc");
		ft_lstclear(&node, free);
		error_exit(data, NULL);
	}
}
