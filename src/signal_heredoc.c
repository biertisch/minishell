/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 12:31:48 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/13 22:05:54 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals_heredoc(t_data *data)
{
	if (isatty(STDIN_FILENO))
	{
		setup_handler(data, SIGINT, heredoc_sigint_handler, 0);
		setup_handler(data, SIGQUIT, SIG_IGN, 0);
	}
	else
	{
		setup_handler(data, SIGINT, SIG_DFL, 0);
		setup_handler(data, SIGQUIT, SIG_DFL, 0);
	}
}

void	heredoc_sigint_handler(int sig)
{
	g_sig = sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

int	heredoc_sigint_abort(t_data *data, char *line, int out_fd)
{
	close(out_fd);
	free(line);
	free_all(data);
	rl_clear_history();
	exit(128 + g_sig);
}

int	heredoc_eof_abort(t_data *data, char *target, int out_fd)
{
	syntax_error(data, SYN_ERR_8, target);
	close(out_fd);
	free_all(data);
	rl_clear_history();
	exit(0);
}
