/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:04:39 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/22 10:22:31 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	setup_signals(t_data *data)
{
	struct sigaction	sa;

	if (isatty(STDIN_FILENO)) //check argc as well?
	{
		sa.sa_handler = signal_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		if (sigaction(SIGINT, &sa, NULL))
		{
			system_error(data, "sigaction");
			error_exit(data);
		}
		sa.sa_handler = SIG_IGN;
		if (sigaction(SIGQUIT, &sa, NULL))
		{
			system_error(data, "sigaction");
			error_exit(data);
		}
		if (sigaction(SIGTERM, &sa, NULL))
		{
			system_error(data, "sigaction");
			error_exit(data);
		}
	}
}

void	signal_handler(int sig)
{
	g_sig_received = sig;
}

void	setup_signals_child(t_data *data)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL))
	{
		system_error(data, "sigaction");
		error_exit(data);
	}
	if (sigaction(SIGQUIT, &sa, NULL))
	{
		system_error(data, "sigaction");
		error_exit(data);
	}
	if (sigaction(SIGTERM, &sa, NULL))
	{
		system_error(data, "sigaction");
		error_exit(data);
	}
}

//readline handler for prompt_continuation
int	rl_sigint_continuation(void)
{
	if (g_sig_received == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
		return (1);
	}
	return (0);
}

//readline handler for prompt_input
int	rl_sigint_main(void)
{
	if (g_sig_received == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	return (0);
}
