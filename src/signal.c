/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:04:39 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/12 12:06:02 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	setup_signals(int argc)
{
	struct sigaction	sa;

	if (argc == 1 && isatty(STDIN_FILENO)) //check argc?
	{
		sa.sa_handler = sigint_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		sigaction(SIGINT, &sa, NULL);
		sa.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &sa, NULL);
		sigaction(SIGTERM, &sa, NULL);
	}
}

void	sigint_handler(int sig)
{
	g_sig_received = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_eof(t_data *data)
{
	if (!ft_strcmp(get_env_value(data->env_list, "SHLVL"), "1"))
		printf("logout\n");
	else
		printf("exit\n");
	free_all(data);
	exit(EXIT_SUCCESS);
}

void	setup_signals_child(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}
