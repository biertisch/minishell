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
# include <errno.h>
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
	NODE_BUILTIN,
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

typedef enum e_phase
{
	ENTERED,
	LAUNCH_LEFT,
	LAUNCH_RIGHT,
	WAIT,
	DONE
}	t_phase;

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

typedef struct s_stack
{
	t_phase		phase;
	t_node_type	type;
	t_tree		*node;
	int			in_fd;
	int			out_fd;
	int			old_fd;
	int			pipe[2];
	pid_t		child_pid[2];
	int		child_count;
	struct s_stack		*next;
}	t_stack;

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
// builtin.c
void		process_builtin(t_data *data, t_tree *head);

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
void		free_stack(t_stack *stack);
void		check_for_errors(int status, t_data *data, t_stack *stack,
				char *command_name);
void		validate_malloc_wildcard(t_data *data, void *ptr, t_list *node,
				char **new_argv);

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

//signal_handler.c
void		setup_signals(void);
void		sigint_handler(int sig);

//executor.c
int			execute(t_data *data);
int			execute_stack(t_data *data, t_stack **stack);
int			execute_cmd(t_data *data, t_stack **stack);
int			execute_pipe(t_data *data, t_stack **stack);
int			execute_pipe_entered(t_data *data, t_stack **stack);
int			execute_pipe_launch_left(t_data *data, t_stack **stack);
int			execute_pipe_launch_right(t_data *data, t_stack **stack);
int			execute_pipe_wait(t_stack **stack);
int			execute_pipe_done(t_stack **stack);
int			execute_cmd_entered(t_data *data, t_stack **stack);
int			execute_cmd_done(t_stack **stack);


//stack.c
t_stack		*create_stack(t_data *data);
void		push_stack(t_stack **stack, t_tree *node, int in_fd, int out_fd, t_data *data);
int			has_pipe_ancestor(t_stack *stack);
//void		print_stack(t_stack *stack);
void		pop(t_stack **stack);
t_stack		**get_first_pipe(t_stack **stack);
void		print_stack(t_stack *stack);

//child.c
void		child(t_data *data, t_stack **stack);
void		child_redir_in(t_data *data, t_stack **stack);
void		child_no_redir(t_data *data, t_stack **stack);
void		child_redir_out(t_data *data, t_stack **stack);

//executor_utils.c
char		*correct_path(t_data * data, char *cmd);
char		*run_curr_dir(char *cmd);

//main.c - needed to uncomment function to print node because i need it lol
void 		print_parser_tree(t_tree *head);
int			is_builtin(char *cmd);

//wildcard.c
int			has_wildcard(const char *arg);
int			expand_wildcard(t_data *data, char *pattern, t_list **entries);
char		*update_redir(t_data *data, char *file, t_list *entry);

//wildcard_argv.c
char		**update_argv(t_data *data, char **argv, int i, t_list *entries);

//wildcard_match.c
int			match_wildcard(char *entry, char *wildcard);

//parent.c
int			parent(t_stack **stack, pid_t pid);

#endif
