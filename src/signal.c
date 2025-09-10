/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:04:39 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/10 15:36:34 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler); //CTRL-C
	//signal(SIGQUIT, sigquit_handler); // CTRL-/
	//signal(SIGTERM, sigterm_handler);
}
//child processes must be restored to default behaviour after fork
// signal(SIGINT, SIG_DFL);
// signal(SIGQUIT, SIG_DFL);

void	sigint_handler(int sig)
{
	(void)sig;
	g_sigint_received = 1;
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
