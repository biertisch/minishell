/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:04:14 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/21 22:53:13 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../include/struct_def.h"
# include "../include/libft.h"
# include "../include/printf.h"
# include "../include/executor.h"
# include "../include/parser.h"
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
# define ERR_9 "arithmetic operations not supported"
# define ERR_10 "semicolon not supported"
# define BUFFER_SIZE 20

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	char			*input;
	char			*prompt;
	char			**env;
	t_env			*env_list;
	t_token			*lexer_list;
	t_tree			*parser_tree;
	t_stack			*stack;
	int				exit_status;
}	t_data;

extern volatile sig_atomic_t	g_sig_received;

//test.c ---- DELETE WHEN COMPLETE
void		print_env_list(t_env *head);
void		print_env_array(char **env);
void		print_lexer_list(t_token *head);
void		print_parser_tree(t_tree *head);
void		test_builtin_validation(t_data *data, t_tree *head);

//cleanup.c
void		free_all(t_data *data);
void		free_command_data(t_data *data);
void		free_redir(t_redir *redir);
void		free_string_array(char ***arr);
void		free_stack(t_stack **stack);

//env.c
int			generate_minimal_env(t_data *data, char **argv);
void		unset_env(t_env **head, char *key);
void		set_env_value(t_env *head, char *key, char *new_value);
char		*get_env_value(t_env *head, char *key);
int			is_valid_var_name(char *s);
int			is_new_var(char *arg);

//env_convert.c
void		env_list_to_array(t_data *data);
int			envp_to_list(t_data *data, char **envp, char **argv);
void		split_env_entry(t_data *data, char *entry, t_env *node);

//env_list.c
void		free_env_list(t_env **head);
void		free_env_node(t_env **node);
t_env		*get_last_env_node(t_env *head);
void		add_env_node(t_env **head, t_env *new_node);
t_env		*create_env_node(char *key, char *value, int exported);

//error.c
int			system_error(t_data *data, char *function);
int			syntax_error(t_data *data, char *desc, char *token);
int			internal_error(t_data *data, char *desc, char *cmd, char *arg);
void		error_exit(t_data *data);
void		validate_malloc(t_data *data, void *ptr, void *to_free);
void		validate_malloc_tree(t_data *data, void *ptr, t_tree *left,
				t_tree *right);
void		validate_malloc_env(t_data *data, void *ptr, t_env *node);
void		check_for_errors(int status, t_data *data, t_stack *stack,
				char *command_name);
void		validate_malloc_wildcard(t_data *data, void *ptr, t_list *node,
				char **new_argv);

//input.c
void		prompt_input(t_data *data);
void		read_input(t_data *data);
int			handle_signal_interruption(t_data *data, char *line, int cont);
void		handle_eof(t_data *data);

//signal.c
void		setup_signals(t_data *data);
void		signal_handler(int sig);
void		setup_signals_child(t_data *data);
int			rl_sigint_main(void);
int			rl_sigint_continuation(void);

//input_prompt.c
void		update_prompt(t_data *data);

//input_continue.c
int			prompt_continuation(t_data *data, char target);

#endif
