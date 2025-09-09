/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:17:33 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/09 10:43:19 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_flag(char *arg)
{
	return ((arg[0] && arg[0] == '-') || (arg[0] && is_quote(arg[0])
			&& arg[1] && arg[1] == '-'));
}

static int	validate_env(t_data *data, char **argv)
{
	if (argv[1] && is_flag(argv[1]))
	{
		internal_error(data, ERR_4, argv[0], argv[1]);
		data->exit_status = 125;
		return (-1);
	}
	if (argv[1])
	{
		internal_error(data, ERR_6, argv[0], argv[1]);
		data->exit_status = 127;
		return (-1);
	}
	return (0);
}

static int	validate_flags(t_data *data, char **argv, char *allowed)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (is_flag(argv[i]) && (!allowed || ft_strcmp(argv[i], allowed)))
			return (internal_error(data, ERR_4, argv[0], argv[i]));
		i++;
	}
	return (0);
}

int	validate_builtin(t_data *data, t_tree *node)
{
	if (!node || node->type != NODE_BUILTIN)
		return (0);
	if (!ft_strcmp(node->argv[0], "pwd") || !ft_strcmp(node->argv[0], "export")
		|| !ft_strcmp(node->argv[0], "unset")
		|| !ft_strcmp(node->argv[0], "exit"))
		if (validate_flags(data, node->argv, NULL))
			return (-1);
	if (!ft_strcmp(node->argv[0], "env"))
		if (validate_env(data, node->argv))
			return (-1);
	if (!ft_strcmp(node->argv[0], "cd"))
	{
		if (node->argv[1] && node->argv[2])
			return (internal_error(data, ERR_5, node->argv[0], NULL));
		if (node->argv[1] && is_flag(node->argv[1]))
			return (internal_error(data, ERR_4, node->argv[0], node->argv[1]));
	}
	return (0);
}
