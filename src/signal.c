/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:04:39 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/06 14:05:55 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals(t_data *data)
{
	if (isatty(STDIN_FILENO))
	{
		setup_handler(data, SIGINT, sigint_handler, 0);
		setup_handler(data, SIGQUIT, SIG_IGN, 0);
	}
	else
	{
		setup_handler(data, SIGINT, SIG_DFL, 0);
		setup_handler(data, SIGQUIT, SIG_DFL, 0);
	}
}

void	setup_signals_parent(t_data *data)
{
	if (isatty(STDIN_FILENO))
	{
		setup_handler(data, SIGINT, SIG_IGN, 0);
		setup_handler(data, SIGQUIT, SIG_IGN, 0);
	}
}

void	setup_signals_cont(t_data *data)
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

void	setup_signals_child(t_data *data)
{
	setup_handler(data, SIGINT, SIG_DFL, 0);
	setup_handler(data, SIGQUIT, SIG_DFL, 0);
}

void	setup_handler(t_data *data, int signum, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	if (sigaction(signum, &sa, NULL))
	{
		system_error(strerror(errno), "sigaction");
		error_exit(data, NULL);
	}
}
