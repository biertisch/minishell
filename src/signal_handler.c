/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 10:25:47 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/13 21:40:34 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	g_sig = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_child_exit(int status)
{
	int	signal;

	if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		if (signal == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (signal == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	}
}

void	handle_child_exit_background(int status)
{
	int	signal;

	if (WIFSIGNALED(status))
	{
		signal = WTERMSIG(status);
		if (signal == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
}

int	sigint_abort(t_data *data, char *line, int cont)
{
	data->exit_status = 128 + g_sig;
	g_sig = 0;
	if (cont)
	{
		free(line);
		return (INCOMPLETE);
	}
	return (VALID);
}

void	eof_abort(t_data *data)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
	free_all(data);
	exit(data->exit_status);
}
