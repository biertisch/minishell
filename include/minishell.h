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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	char			*input;
	char			**env;
	t_env			*env_list;
	t_token			*lexer_list;
	t_tree			*parser_tree;
	t_stack			*stack;
	int				exit_status;
}	t_data;

//cleanup.c
void		free_all(t_data *data);
void		free_command_data(t_data *data);
void		free_string_array(char ***arr);
void		free_stack(t_stack **stack);

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
void		check_for_errors(int status, t_data *data, t_stack *stack,
				char *command_name);
void		validate_malloc_wildcard(t_data *data, void *ptr, t_list *node,
				char **new_argv);

//signal_handler.c
void		setup_signals(void);
void		sigint_handler(int sig);

//main.c
void 		print_parser_tree(t_tree *head);
int			is_builtin(char *cmd);

#endif
