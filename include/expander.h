/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:21:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/21 23:23:28 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"

//expander.c
int			expand(t_data *data, t_tree *node);

//expand_tilde.c
void		expand_tilde(t_data *data, char **arg);

//expander_dollar.c
char		**expand_dollar(t_data *data, char **arg, int index);
void		expand_exit_status(t_data *data, char **arg, int i);
char		*get_env_key(char *arg);
char		*update_arg(char *arg, int i, char *key, char *value);

//expander_dollar2.c
char		**update_argv_dollar(char **old_argv, int i, char *value);

//expander_dollar3.c
int			expand_dollar_redir(t_data *data, char **file);

//expander_quotes.c
void		remove_quotes(t_data *data, char **arg);

//wildcard.c
int			has_wildcard(const char *arg);
int			expand_wildcard(t_data *data, char *pattern, t_list **entries);
char		*update_redir_wildcard(t_data *data, char *file, t_list *entry);

//wildcard_argv.c
char		**update_argv_wildcard(t_data *data, char **argv, int i,
				t_list *entries);

//wildcard_match.c
int			match_wildcard(char *entry, char *wildcard);

#endif