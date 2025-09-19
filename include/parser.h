/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:58:05 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/19 10:58:06 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../include/minishell.h"

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	int				fd;
	char			*file;
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

//expander.c
int			expand(t_data *data, t_tree *node);

//expander_dollar.c
void		expand_dollar(t_data *data, char **arg);

//expander_quotes.c
void		remove_quotes(t_data *data, char **arg);

//lexer.c
int			lexer(t_data *data);

//lexer_list.c
void		free_lexer_list(t_token **head);
void		free_lexer_node(t_token **node);
t_token		*get_last_lexer_node(t_token *head);
void		add_lexer_node(t_token **head, t_token *new_node);
t_token		*create_lexer_node(t_token_type type, char *value);

//parser.c
int			parser(t_data *data);
t_tree		*parse_and_or(t_data *data, t_token **token);

//parser_cmd.c
int			get_command_data(t_data *data, t_token **token, t_tree *node);
int			is_command_token(t_token_type token_type);

//parser_redir.c
t_redir		*get_redir(t_data *data, t_token **token);
int			is_redir_token(t_token_type token_type);

//parser_tree.c
void		free_parser_tree(t_tree **node);
void		free_parser_node(t_tree **node);
void		free_redir(t_redir *redir);
t_tree		*create_parser_node(t_node_type type, t_tree *left, t_tree *right);
int			count_tree_nodes(t_tree *root);

//parser_utils.c
t_node_type	get_node_type(t_token_type token_type);
int			is_operator(char *s);
int			is_quote(char c);
int			is_fd(char *input);

//wildcard.c
int			has_wildcard(const char *arg);
int			expand_wildcard(t_data *data, char *pattern, t_list **entries);
char		*update_redir(t_data *data, char *file, t_list *entry);

//wildcard_argv.c
char		**update_argv(t_data *data, char **argv, int i, t_list *entries);

//wildcard_match.c
int			match_wildcard(char *entry, char *wildcard);

#endif
