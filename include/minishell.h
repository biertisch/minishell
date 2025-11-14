/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:04:14 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/14 11:14:56 by beatde-a         ###   ########.fr       */
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
# include "libft.h"
# include "printf.h"
# include "structures.h"
# include "environment.h"
# include "errors.h"
# include "executor.h"
# include "expander.h"
# include "lexer.h"
# include "parser.h"
# include "signals.h"

# define CONTINUE_PROMPT "> "
# define BUFFER_SIZE 20
# define DEFAULT_PATH "/usr/local/sbin:/usr/local/bin:\
/usr/sbin:/usr/bin:/sbin:/bin"

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
void		free_metadata(t_metadata **info, int size);

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
int			read_input(t_data *data);
int			process_input(t_data *data);
int			read_continuation_input(t_data *data, char target, int out_fd);
int			handle_incomplete_input(t_data *data);

//input_continuation.c
int			run_incomplete_child(t_data *data, char target, int *pipe_fd);
int			run_incomplete_parent(t_data *data, int *pipe_fd, pid_t pid,
				char **input);
char		*receive_continuation_input(t_data *data, int in_fd);
int			rebuild_tree(t_data *data, char *cont_input);
int			merge_sub_tree(t_data *data, t_tree *sub_tree);

//input_prompt.c
void		update_prompt(t_data *data);
char		*get_prompt_pwd(t_data *data);

#endif
