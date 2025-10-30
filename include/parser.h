/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:58:05 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/30 13:55:52 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

typedef struct s_redir
{
	t_token_type	type;
	int				fd;
	char			*file;
	char			*heredoc_input;
	int				in_fd;
	int				out_fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_tree
{
	t_node_type		type;
	char			**argv;
	t_redir			*redir;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

//parser.c
int			parser(t_data *data);
int			parse_and_or(t_data *data, t_token **token, t_tree **root);
int			parse_pipe(t_data *data, t_token **token, t_tree **root);
int			parse_command(t_data *data, t_token **token, t_tree **root);
int			parse_subshell(t_data *data, t_token **token, t_tree **root);

//parser_cmd.c
int			get_command_data(t_data *data, t_token **token, t_tree *node);
char		**allocate_argv(t_data *data, t_token **token, t_tree *node);
int			is_command_token(t_token_type token_type);
void		get_word(t_data *data, t_token **token, t_tree *node, int *i);
int			count_argv(t_token *token);

//parser_redir.c
int			is_redir_token(t_token_type token_type);
int			get_redir(t_data *data, t_token **token, t_tree *node);
t_redir		*parse_single_redir(t_data *data, t_token **token, t_redir *head);
t_redir		*create_redir(t_token_type type, int fd, char *file);

//parser_subshell.c
int			empty_subshell(t_token **token, t_tree *node, int res);
int			invalid_sequence(t_data *data, t_token *token, t_tree *node);

//parser_tree.c
t_tree		*create_parser_node(t_node_type type, t_tree *left, t_tree *right);
void		free_parser_node(t_tree **node);
void		free_parser_tree(t_tree **root);
int			push_left_until_cmd(t_data *data);
int			count_tree_nodes(t_tree *root);

//parser_utils.c
t_node_type	get_node_type(t_token_type token_type);
int			is_builtin(char *cmd);

#endif
