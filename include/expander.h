/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:21:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 17:33:29 by beatde-a         ###   ########.fr       */
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
int			expand_dollar_redir(t_data *data, t_redir *redir);
int			expand_wildcard_redir(t_data *data, t_redir *redir);

//expander_dollar.c
int			is_dollar_expansion(char *arg, int *quote_map, int start);
int			expand_dollar(t_data *data, char **arg, t_metadata *info);
int			get_key_value(t_data *data, char *arg, t_metadata *info, int i);
char		*expand_variable(t_data *data, char *arg, t_metadata *info, int i);
int			rebuild_quote_map(t_data *data, t_metadata *info, int start);
int			rebuild_expand_map(t_data *data, t_metadata *info, int start, int type);
char		*apply_expansion(char *src, t_metadata *info, int start);
char		*get_env_key(char *arg, int *quote_map, int start);
int			handle_empty_quote_map(t_data *data, t_metadata *info, int start, int old_len);
int			get_quote_status(int *quote_map, int start, int len);
int			handle_empty_expand_map(t_data *data, t_metadata *info);
int			init_expand_metadata(t_data *data, t_metadata *info);

//expander_quotes.c
int			remove_quotes(t_data *data, char **arg, t_metadata *info);
int			count_quotes(char *arg);
int			copy_without_quotes(char *dest, char *src, int *map);
int			get_quote_map(t_data *data, char *arg, t_metadata *info);

//expand_tilde.c
int			has_tilde(char *arg, int quote_status);
int			expand_tilde(t_data *data, char **arg, t_metadata *info);
int			update_tilde_key_value(t_data *data, char *arg, t_metadata *info);
char		*get_tilde_key(char *arg);
char		*get_tilde_value(t_data *data, char *key);

//wildcard.c
int			has_wildcard(char *arg, t_metadata *info);
int			expand_wildcard(t_data *data, t_tree *node);
int			expand_single_wildcard(t_data *data, char *arg, t_list **entries);
t_list		*get_entries(t_data *data, DIR *dir_stream);
void		filter_matches(t_list **head, char *pattern);
char		*apply_redir_wildcard(t_data *data, char *file, t_list *entry);

//wildcard_argv.c
int			rebuild_argv_wildcard(t_tree *node, t_list *entries, int star_index);
int			handle_malloc_failure_wildcard(char **argv, t_metadata *info, int argc);
int			copy_entries(char **argv, t_metadata *info, t_list *entries, int start);
int			copy_before_star(t_tree *node, char **argv, t_metadata *info, int size);
int			copy_after_star(t_tree *node, char **argv, t_metadata *info, int start);

//wildcard_match.c
int			match_wildcard(char *entry, char *wildcard);
void		advance_both(int *i, int *j);
void		record_star(int *j, int *star);
int			backtrack_on_mismatch(int *i, int *j, int *star);

//expander_resize.c
int			get_argc(char **argv);
int			get_expanded_argc(char **argv, t_metadata *info);
char 		**rebuild_argv(t_data *data, char **argv, t_metadata **info);
int			arg_disappears(char *arg, t_metadata *info);
int			is_ifs(char *arg, t_metadata *info, int i);
int			count_fields(char *arg, t_metadata *info);
int			rebuild_argv(t_data *data, char **dest, char **src, t_metadata *src_info, t_metadata *dest_info);
int			has_ifs(char *arg, t_metadata *info, int i);
int			split_on_ifs(char **dest, char *src, t_metadata *src_info, t_metadata *dest_info);
int			copy_arg_info(t_metadata *dest, t_metadata *src, int start, int end);

//expander_utils.c
char		**copy_string_array(char **src);
void		free_unfinished_string_array(char **arr, int size);

#endif