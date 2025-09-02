/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 11:22:28 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/01 11:22:29 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler); //CTRL-C
	//signal(SIGQUIT, sigquit_handler); // CTRL-/
	//signal(SIGTERM, sigterm_handler);
}

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\nminishell$ ", 13);
}
