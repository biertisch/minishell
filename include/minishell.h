/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:04:14 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/02 12:12:10 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include "struct_def.h"
# include "libft.h"
# include "printf.h"
# include "executor.h"
# include "parser.h"
# include "expander.h"
# include "lexer.h"
# include "ft_signal.h"
# include "env.h"

# define CONTINUE_PROMPT "> "
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
# define ERR_11 "minishell: warning: here-document delimited by \
end-of-file (wanted '"
# define BUFFER_SIZE 20

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

extern volatile sig_atomic_t	g_sig;

//test.c ---- DELETE WHEN COMPLETE
void		print_env_list(t_env *head);
void		print_env_array(char **env);
void		print_lexer_list(t_token *head);
void		print_parser_tree(t_tree *head);
void		print_parser_node(t_tree *node, int depth, char *pos);
void		test_builtin_validation(t_data *data, t_tree *head);

//cleanup.c
void		free_all(t_data *data);
void		free_command_data(t_data *data);
void		free_stack(t_stack **stack);
void		free_redir(t_redir *redir);
void		free_string_array(char ***arr);

//error.c
int			system_error(t_data *data, char *function);
int			syntax_error(t_data *data, char *desc, char *token);
int			internal_error(t_data *data, char *desc, char *cmd, char *arg);
void		error_exit(t_data *data, t_stack **stack);
void		validate_malloc(t_data *data, void *ptr, void *to_free);
void		validate_malloc_tree(t_data *data, void *ptr, t_tree *left,
				t_tree *right);
void		validate_malloc_env(t_data *data, void *ptr, t_env *node);
void		check_for_errors(int status, t_data *data, t_stack *stack,
				char *command_name);
void		validate_malloc_wildcard(t_data *data, void *ptr, t_list *node,
				char *new_arg);
void		validate_malloc_execute(t_data *data, t_stack **stack, void *ptr,
				void *to_free);

//get_next_line.c
char		*get_next_line(int fd);

//get_next_line_utils.c
int			ft_find_init_nl(char *buff);
char		*ft_handle_new_line(char *buff, char *result, int i);
char		*ft_process_buffer(char *buff, int i);
char		*get_next_line_cont(int fd, char *buffer, char *result,
				int bytes_read);
char		*ft_gnl_realloc(char *result, int i, int *r);

//input.c
int			prompt_input(t_data *data);
int			read_input(t_data *data);
int			process_input(t_data *data);
int			prompt_input_cont(t_data *data, char target, int fd);

//input_cont.c
int			handle_incomplete_input(t_data *data, char target);
int			run_incomplete_child(t_data *data, char target, int *pipe_fd);
int			write_to_pipe(char *line, char target, int fd);
int			run_incomplete_parent(t_data *data, int *pipe_fd, pid_t pid);
char		*copy_continuation_input(t_data *data, int *pipe_fd);

//input_prompt.c
void		update_prompt(t_data *data);
char		*get_env_value_modified(t_data *data, char *key);
char		*get_prompt_pwd(t_data *data);
void		build_prompt(char *prompt, char *shell, char *pwd);

#endif
