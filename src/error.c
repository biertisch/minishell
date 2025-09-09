/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:16:22 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/09 15:25:10 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	error_exit(t_data *data)
{
	write(2, "fatal error: leaving minishell...\n", 34);
	free_all(data);
	exit(EXIT_FAILURE);
}

int	internal_error(t_data *data, char *desc, char *cmd, char *arg)
{
	write(2, "minishell: ", 11);
	if (cmd)
	{
		write(2, cmd, ft_strlen(cmd));
		write(2, ": ", 2);
	}
	if (arg && arg[0] != '\'')
	{
		write(2, arg, ft_strlen(arg));
		write(2, ": ", 2);
	}
	write(2, desc, ft_strlen(desc));
	if (arg && arg[0] == '\'')
	{
		write(2, " ", 1);
		write(2, arg, ft_strlen(arg));
	}
	write(2, "\n", 1);
	data->exit_status = 1;
	return (INVALID);
}

int	syntax_error(t_data *data, char *desc, char *token)
{
	write(2, "minishell: ", 11);
	write(2, desc, ft_strlen(desc));
	if (token)
	{
		write(2, " '", 2);
		write(2, token, ft_strlen(token));
		write(2, "'", 1);
	}
	write(2, "\n", 1);
	data->exit_status = 2;
	return (INVALID);
}

int	system_error(t_data *data, char *function)
{
	write(2, "minishell: ", 11);
	perror(function);
	data->exit_status = 1;
	return (INVALID);
}
