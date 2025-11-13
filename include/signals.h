/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:30:49 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/13 22:06:22 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"

//signal.c
void		setup_signals(t_data *data);
void		setup_signals_cont(t_data *data);
void		setup_signals_child(t_data *data);
void		setup_signals_parent(t_data *data);
void		setup_handler(t_data *data, int signum, void (*handler)(int),
				int flags);

//signal_handler.c
void		sigint_handler(int sig);
void		handle_child_exit(int status);
void		handle_child_exit_background(int status);
int			sigint_abort(t_data *data, char *line, int cont);
void		eof_abort(t_data *data);

//signal_heredoc.c
void		setup_signals_heredoc(t_data *data);
void		heredoc_sigint_handler(int sig);
int			heredoc_sigint_abort(t_data *data, char *line, int out_fd);
int			heredoc_eof_abort(t_data *data, char *target, int out_fd);

#endif
