/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:04:14 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/20 10:04:14 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//HEADERS
# include "../include/libft.h"
# include "../include/printf.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termcap.h>
# include <termios.h>
# include <unistd.h>

//ENUMS
typedef enum e_token_type
{
	WORD,
	PIPE,
	AND,
	OR,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	FD,
	LPAREN,
	RPAREN
}	t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef enum e_error
{
	SYSTEM_ERR,
	SYNTAX_ERR,
	INTERNAL_ERR
}	t_error;

//STRUCTS
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

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

//maybe add struct with int *fd_opened and int fd_open_counter

typedef struct s_data
{
	char			*input;
	char			**env;
	t_env			*env_list;
	int				exit_status;
	t_token			*lexer_list;
	t_tree			*parser_tree;
}	t_data;

//PROTOTYPES
//cleanup.c
void		free_all(t_data *data);
void		free_command_data(t_data *data);
void		free_string_array(char ***arr);

//env.c
void		unset_env(t_env **head, char *key);
void		set_env_value(t_env *head, char *key, char *new_value);
char		*get_env_value(t_env *head, char *key);
char		*get_env_key(char *arg);
char		*replace_key_value(char *arg, int i, char *key, char *value);

//env_convert.c
void		env_list_to_array(t_data *data);
void		envp_to_list(t_data *data, char **envp);

//env_list.c
void		free_env_list(t_env **head);
void		free_env_node(t_env **node);
t_env		*get_last_env_node(t_env *head);
void		add_env_node(t_env **head, t_env *new_node);
t_env		*create_env_node(void);

//error.c
int			report_error(char *error_msg, t_error error_code);
void		error_exit(t_data *data);
void		validate_malloc(t_data *data, void *ptr, void *to_free);
void		validate_malloc_tree(t_data *data, void *ptr, t_tree *left,
				t_tree *right);
void		validate_malloc_env(t_data *data, void *ptr, t_env *node);
void		validate_malloc_wildcard(t_data *data, void *ptr, t_list *node,
				DIR *dir_stream);

//expander.c
int			expand(t_data *data, t_tree *node);

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

//parser_utils.c
t_node_type	get_node_type(t_token_type token_type);
int			is_operator(char *s);
int			is_quote(char c);
int			is_fd(char *input);

//wildcard.c
int			expand_wildcard(t_data *data, char ***argv, int i);
int			has_wildcard(const char *arg);

//wildcard_match.c
int			match_wildcard(char *entry, char *wildcard);

#endif
