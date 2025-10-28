/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:30:49 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 16:32:28 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H

# include "minishell.h"

//signal.c
void		setup_signals(t_data *data);
void		setup_signals_cont(t_data *data);
void		setup_signals_command(t_data *data);
void		setup_signals_heredoc(t_data *data);
void		setup_signals_parent(t_data *data);

//signal_handler.c
void		setup_handler(t_data *data, int signum, void (*handler)(int),
				int flags);
void		sigint_handler(int sig);
int			sigint_abort(t_data *data, char *line, int cont);
void		heredoc_sigint_handler(int sig);
int			heredoc_sigint_abort(t_data *data, char *line);

//signal_eof.c
void		eof_abort(t_data *data);
int			heredoc_eof_abort(t_data *data, char *target);

#endif
