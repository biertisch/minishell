/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 10:25:47 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 12:50:48 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_handler(t_data *data, int signum, void (*handler)(int), int flags)
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = flags;
	if (sigaction(signum, &sa, NULL))
	{
		system_error(data, "sigaction");
		error_exit(data);
	}
}

void	sigint_handler(int sig)
{
	g_sig = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	sigint_abort(t_data *data, char *line, int cont)
{
	data->exit_status = 128 + g_sig;
	g_sig = 0;
	if (cont)
	{
		free_command_data(data);
		free(line);
		return (INCOMPLETE);
	}
	return (VALID);
}

void	heredoc_sigint_handler(int sig)
{
	g_sig = sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

int		heredoc_sigint_abort(t_data *data, char *line)
{
	data->exit_status = 128 + g_sig;
	g_sig = 0;
	free_command_data(data);
	free(line);
	return (INVALID);
}
