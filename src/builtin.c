/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:17:33 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/11 15:05:31 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	print_usage(char *cmd)
{
	write(2, cmd, ft_strlen(cmd));
	write(2, ": usage: ", 9);
	write(2, cmd, ft_strlen(cmd));
	if (!ft_strcmp(cmd, "cd"))
		write(2, " [dir]", 6);
	else if (!ft_strcmp(cmd, "export"))
		write(2, " [name[=value]...]", 19);
	else if (!ft_strcmp(cmd, "unset"))
		write(2, " [name...]", 10);
	write(2, "\n", 1);
	return (INVALID);
}

static int	validate_env(t_data *data, char **argv)
{
	if (argv[1] && argv[1][0] == '-')
	{
		internal_error(data, ERR_4, argv[0], argv[1]);
		data->exit_status = 125;
		print_usage(argv[0]);
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
		if (argv[i][0] == '-' && (!allowed || ft_strcmp(argv[i], allowed)))
		{
			internal_error(data, ERR_4, argv[0], argv[i]);
			return (print_usage(argv[0]));
		}
		i++;
	}
	return (0);
}

int	validate_builtin(t_data *data, t_tree *node)
{
	if (!node || node->type != NODE_BUILTIN)
		return (0);
	if (!ft_strcmp(node->argv[0], "pwd") || !ft_strcmp(node->argv[0], "export")
		|| !ft_strcmp(node->argv[0], "unset"))
		if (validate_flags(data, node->argv, NULL))
			return (-1);
	if (!ft_strcmp(node->argv[0], "env") && validate_env(data, node->argv))
		return (-1);
	if (!ft_strcmp(node->argv[0], "cd"))
	{
		if (node->argv[1] && node->argv[2])
			return (internal_error(data, ERR_5, node->argv[0], NULL));
		if (node->argv[1] && node->argv[1][0] == '-')
		{
			internal_error(data, ERR_4, node->argv[0], node->argv[1]);
			return (print_usage(node->argv[0]));
		}
	}
	return (0);
}
