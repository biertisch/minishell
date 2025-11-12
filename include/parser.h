/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:58:05 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/12 12:10:09 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

//parser.c
int			parser(t_data *data, t_token *token, t_tree **parser_tree);
int			parse_and_or(t_data *data, t_token **token, t_tree **root);
int			parse_pipe(t_data *data, t_token **token, t_tree **root);
int			parse_command(t_data *data, t_token **token, t_tree **root);
int			parse_subshell(t_data *data, t_token **token, t_tree **root);

//parser_cmd.c
int			get_command_data(t_data *data, t_token **token, t_tree *node);
char		**allocate_argv(t_data *data, t_token **token, t_tree *node);
void		get_arg(t_data *data, t_token **token, t_tree *node, int *i);
int			count_argv(t_token *token);

//parser_heredoc_collect.c
int			run_heredoc_child(t_data *data, t_redir *redir, int *pipe_fd);
int			collect_heredoc(t_data *data, t_redir *redir, int out_fd);
int			run_heredoc_parent(t_data *data, t_redir *redir, int *pipe_fd,
				pid_t pid);
int			copy_heredoc_input(t_data *data, t_redir *redir, int in_fd);
int			wait_heredoc(t_data *data, pid_t pid);

//parser_heredoc_scan.c
int			scan_heredocs(t_data *data, t_tree *parser_tree);
int			scan_heredocs_right(t_data *data);
int			scan_heredocs_left(t_data *data);
int			setup_heredoc(t_data *data, t_redir *redir);
t_redir		*get_last_heredoc(t_redir *redir);

//parser_redir.c
int			get_redir(t_data *data, t_token **token, t_tree *node);
t_redir		*parse_single_redir(t_data *data, t_token **token, t_redir *head);
t_redir		*create_redir(t_token_type type, int fd, char *file);
void		free_redir(t_redir *redir);

//parser_tree.c
t_tree		*create_parser_node(t_node_type type, t_tree *left, t_tree *right);
void		free_parser_node(t_tree **node);
void		free_parser_tree(t_tree **root);
int			push_left_until_cmd(t_data *data);
int			count_tree_nodes(t_tree *root);

//parser_utils.c
t_node_type	get_node_type(t_token_type token_type);
int			is_redir_token(t_token_type token_type);
int			empty_subshell(t_token **token, t_tree **root,
				t_tree *node, int res);
int			is_command_token(t_token_type token_type);
int			parse_sub_tree(t_data *data, char *input, t_token *token,
				t_tree **root);

#endif
