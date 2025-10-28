/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_eof.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 12:38:03 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 13:02:42 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	eof_abort(t_data *data)
{
	if (isatty(STDIN_FILENO))
		write(1, "exit\n", 5);
	free_all(data);
	exit(data->exit_status);
}

int		heredoc_eof_abort(t_data *data, char *target)
{
	char *tmp;
	char *error_msg;

	tmp = ft_strjoin("minishell: warning: here-document delimited by end-of-file (wanted '", target);
	validate_malloc(data, tmp, NULL);
	error_msg = ft_strjoin(tmp, "')\n");
	validate_malloc(data, error_msg, tmp);
	free(tmp);
	write(STDOUT_FILENO, error_msg, ft_strlen(error_msg));
	free(error_msg);
	return (0);
}
