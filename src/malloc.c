/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:24:57 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/09 15:25:14 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	validate_malloc(t_data *data, void *ptr, void *to_free)
{
	if (!ptr)
	{
		system_error(data, "malloc");
		free(to_free);
		error_exit(data);
	}
}

void	validate_malloc_env(t_data *data, void *ptr, t_env *node)
{
	if (!ptr)
	{
		system_error(data, "malloc");
		free_env_node(&node);
		error_exit(data);
	}
}

void	validate_malloc_tree(t_data *data, void *ptr, t_tree *left,
	t_tree *right)
{
	if (!ptr)
	{
		system_error(data, "malloc");
		free_parser_tree(&left);
		free_parser_tree(&right);
		error_exit(data);
	}
}

void	validate_malloc_wildcard(t_data *data, void *ptr, t_list *node,
	char **new_argv)
{
	if (!ptr)
	{
		system_error(data, "malloc");
		ft_lstclear(&node, free);
		free_string_array(&new_argv);
		error_exit(data);
	}
}
