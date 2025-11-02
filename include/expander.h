/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:21:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/02 15:26:29 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"

//expander.c
int			expand(t_data *data, t_tree *node);
int			expand_argv(t_data *data, t_tree *node);
int			expand_single_arg(t_data *data, char **arg, char *raw_arg);
int			expand_redir(t_data *data, t_tree *node);
int			expand_single_redir(t_data *data, t_redir *redir);

//expander_dollar.c
int			is_dollar_expansion(char quote, char *arg);
int			has_dollar(char *arg);
int			expand_dollar(t_data *data, char **arg);
char		*expand_exit_status(t_data *data, char *old_arg, int i);
char		*expand_variable(t_data *data, char *old_arg, int i);
char		*get_env_key(char *arg);
char		*update_arg(char *arg, int i, char *key, char *value);

//expander_dollar2.c
char		**update_argv_dollar(char **old_argv, int i, char *value);
char		**get_add_argv(char *value);
int			get_argv_count(char **old_argv, char **add_argv, int *add_count);
int			build_argv(char **new_argv, char **old_argv, char **add_argv,
				int i);
int			copy_to_array(char **dest, char **src, int size);

//expander_dollar3.c
int			expand_dollar_redir(t_data *data, char **file);
int			expand_redir_variable(t_data *data, char **file, int i);
int			expand_heredoc_var(t_data *data, char **input, int i);
int			expand_heredoc_input(t_data *data, t_redir *redir);

//expander_quotes.c
void		remove_quotes(t_data *data, char **arg, char *raw_arg);
int			count_quotes(char *arg);
void		pass_expanded_segment(t_data *data, char *raw, int *i, int *j);
char		*copy_without_quotes(t_data *data, char *src, int *to_remove);

//expand_tilde.c
int			has_tilde(char *arg);
int			expand_tilde(t_data *data, char **arg);
char		*get_tilde_key(char *arg);
char		*get_tilde_value(t_data *data, char *key);

//wildcard.c
int			has_wildcard(const char *arg);
int			expand_wildcard(t_data *data, char **arg);
t_list		*get_entries(t_data *data, DIR *dir_stream);
void		filter_matches(t_list **head, char *pattern);
char		*update_arg_wildcard(t_data *data, char *old_arg, t_list *entries);
char		*append_entry(char *arg, char *entry);


char		*update_redir_wildcard(t_data *data, char *file, t_list *entry);

//wildcard_argv.c
char		**update_argv_wildcard(t_data *data, char **argv, int i,
				t_list *entries);
int			count_expanded_argv(char **argv, t_list *head);
int			copy_entries(char **new_argv, t_list *entries, int j);
int			copy_before_star(char **new_argv, char **argv, int i);
int			copy_after_star(char **new_argv, char **argv, int i, int j);

//wildcard_match.c
int			match_wildcard(char *entry, char *wildcard);
void		advance_both(int *i, int *j);
void		record_star(int *j, int *star);
int			backtrack_on_mismatch(int *i, int *j, int *star);


//expander_resize.c
int			get_argc(char **argv);

//expander_utils.c
char		**copy_string_array(char **src);
void		free_unfinished_string_array(char **arr, int size);

#endif