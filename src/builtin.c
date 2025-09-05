/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:17:33 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/05 15:45:58 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_flag(char *arg)
{
	return ((arg[0] && arg[0] == '-') || (arg[0] && is_quote(arg[0])
			&& arg[1] && arg[1] == '-'));
}

static int	validate_flags(char **argv, char *allowed)
{
	while (*argv)
	{
		if (is_flag(*argv) && (!allowed || ft_strcmp(*argv, allowed)))
			return (-1);
		argv++;
	}
	return (0);
}

int	validate_builtin(t_tree *node)
{
	if (!node || node->type != NODE_BUILTIN)
		return (0);
	if (!ft_strcmp(node->argv[0], "echo"))
		if (validate_flags(node->argv, "-n"))
			return (report_error("unsupported command option", INTERNAL_ERR));
	if (!ft_strcmp(node->argv[0], "pwd") || !ft_strcmp(node->argv[0], "export")
		|| !ft_strcmp(node->argv[0], "unset")
		|| !ft_strcmp(node->argv[0], "exit"))
		if (validate_flags(node->argv, NULL))
			return (report_error("unsupported command option", INTERNAL_ERR));
	if (!ft_strcmp(node->argv[0], "env"))
		if (node->argv[1])
			return (report_error("unsupported command option", INTERNAL_ERR));
	if (!ft_strcmp(node->argv[0], "cd"))
		if (node->argv[1] && (is_flag(node->argv[1]) || node->argv[2]))
			return (report_error("unsupported command option", INTERNAL_ERR));
	return (0);
}
