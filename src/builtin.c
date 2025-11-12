/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:17:33 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/11 21:42:15 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

int	is_builtin_no_fork(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

int	has_builtin_flag(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i][0] == '-')
			return (1);
		i++;
	}
	return (0);
}

int	print_builtin_usage(char *cmd)
{
	char	usage[ERR_BUFFER_SIZE];

	usage[0] = '\0';
	ft_strlcpy(usage, cmd, ft_strlen(cmd) + 1);
	ft_strlcat(usage, ": usage: ", ERR_BUFFER_SIZE);
	ft_strlcat(usage, cmd, ERR_BUFFER_SIZE);
	if (!ft_strcmp(cmd, "cd"))
		ft_strlcat(usage, " [dir]", ERR_BUFFER_SIZE);
	else if (!ft_strcmp(cmd, "export"))
		ft_strlcat(usage, " [name[=value]...]", ERR_BUFFER_SIZE);
	else if (!ft_strcmp(cmd, "unset"))
		ft_strlcat(usage, " [name...]", ERR_BUFFER_SIZE);
	ft_strlcat(usage, "\n", ERR_BUFFER_SIZE);
	write(STDERR_FILENO, usage, ft_strlen(usage));
	return (INVALID);
}
