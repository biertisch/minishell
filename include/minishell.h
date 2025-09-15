/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:04:14 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/15 13:55:06 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# define PROMPT "minishell$ "
# define CONTINUE_PROMPT "> "
# define VALID 0
# define INVALID 1
# define INCOMPLETE 2
# define INCOMPLETE_EOF 3
# define ERR_0 "invalid environment variable"
# define ERR_1 "syntax error near unexpected token"
# define ERR_2 "ambiguous redirect"
# define ERR_3 "invalid option"
# define ERR_4 "too many arguments"
# define ERR_5 "No such file or directory"
# define ERR_6 "unexpected EOF while looking for matching"
# define ERR_7 "syntax error: unexpected end of file"
# define ERR_8 "syntax error: missing quote"

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
	NODE_BUILTIN,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

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

extern volatile sig_atomic_t	g_sig_received;

//test.c ---- DELETE WHEN COMPLETE
void		print_env_list(t_env *head);
void		print_env_array(char **env);
void		print_lexer_list(t_token *head);
void		print_parser_tree(t_tree *head);
void		test_builtin_validation(t_data *data, t_tree *head);

// builtin.c
int			validate_builtin(t_data *data, t_tree *node);

//cleanup.c
void		free_all(t_data *data);
void		free_command_data(t_data *data);
void		free_string_array(char ***arr);

//env.c
int			generate_minimal_env(t_data *data, char **argv);
void		unset_env(t_env **head, char *key);
void		set_env_value(t_env *head, char *key, char *new_value);
char		*get_env_value(t_env *head, char *key);

//env_convert.c
void		env_list_to_array(t_data *data);
int			envp_to_list(t_data *data, char **envp, char **argv);

//env_list.c
void		free_env_list(t_env **head);
void		free_env_node(t_env **node);
t_env		*get_last_env_node(t_env *head);
void		add_env_node(t_env **head, t_env *new_node);
t_env		*create_env_node(char *key, char *value);

//error.c
int			system_error(t_data *data, char *function);
int			syntax_error(t_data *data, char *desc, char *token);
int			internal_error(t_data *data, char *desc, char *cmd, char *arg);
void		error_exit(t_data *data);

//expander.c
int			expand(t_data *data, t_tree *node);

//expander_dollar.c
void		expand_dollar(t_data *data, char **arg);

//expander_quotes.c
void		remove_quotes(t_data *data, char **arg);

//input.c
void		prompt_input(t_data *data);
int			process_input(t_data *data);
int			prompt_continuation(t_data *data, char target);

//get_next_line.c
char		*get_next_line(int fd);

//lexer.c
int			lexer(t_data *data);

//lexer_list.c
void		free_lexer_list(t_token **head);
void		free_lexer_node(t_token **node);
t_token		*get_last_lexer_node(t_token *head);
void		add_lexer_node(t_token **head, t_token *new_node);
t_token		*create_lexer_node(t_token_type type, char *value);

//lexer_utils.c
int			is_fd(char *input);
int			is_quote(char c);
int			is_operator(char *s);
int			get_operator_len(char *s);

//malloc.c
void		validate_malloc(t_data *data, void *ptr, void *to_free);
void		validate_malloc_tree(t_data *data, void *ptr, t_tree *left,
				t_tree *right);
void		validate_malloc_env(t_data *data, void *ptr, t_env *node);
void		validate_malloc_wildcard(t_data *data, void *ptr, t_list *node,
				char **new_argv);

//parser.c
int			parser(t_data *data);
int			parse_and_or(t_data *data, t_token **token, t_tree **root);

//parser_cmd.c
int			get_command_data(t_data *data, t_token **token, t_tree *node);
int			is_command_token(t_token_type token_type);

//parser_redir.c
int			get_redir(t_data *data, t_token **token, t_tree *node);
int			is_redir_token(t_token_type token_type);

//parser_subshell.c
int			empty_subshell(t_token **token, t_tree *node, int res);
int			invalid_sequence(t_data *data, t_token *token, t_tree *node);

//parser_tree.c
void		free_parser_tree(t_tree **node);
void		free_parser_node(t_tree **node);
void		free_redir(t_redir *redir);
t_tree		*create_parser_node(t_node_type type, t_tree *left, t_tree *right);

//parser_utils.c
t_node_type	get_node_type(t_token_type token_type);
int			is_builtin(char *cmd);

//signal.c
void		setup_signals(int argc);
void		sigint_handler(int sig);
void		handle_eof(t_data *data);
void		setup_signals_child(void);

//wildcard.c
int			has_wildcard(const char *arg);
int			expand_wildcard(t_data *data, char *pattern, t_list **entries);
char		*update_redir(t_data *data, char *file, t_list *entry);

//wildcard_argv.c
char		**update_argv(t_data *data, char **argv, int i, t_list *entries);

//wildcard_match.c
int			match_wildcard(char *entry, char *wildcard);

#endif
