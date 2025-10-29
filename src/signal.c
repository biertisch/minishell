/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:04:39 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 19:03:57 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	setup_signals_command(t_data *data)
{
	setup_handler(data, SIGINT, SIG_DFL, 0);
	setup_handler(data, SIGQUIT, SIG_DFL, 0);
}

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

void	setup_signals_parent(t_data *data)
{
	if (isatty(STDIN_FILENO))
	{
		setup_handler(data, SIGINT, SIG_IGN, 0);
		setup_handler(data, SIGQUIT, SIG_IGN, 0);
	}
}
