/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:55:16 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/19 10:55:17 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../include/minishell.h"

typedef struct s_stack
{
	t_phase		phase;
	t_node_type	type;
	t_tree		*node;
	int			in_fd;
	int			out_fd;
	int			pipe[2];
	int			child_count;
	int			exit_status;
	pid_t		child_pid[2];
	struct s_stack		*next;
	char		*real_cmd;
}	t_stack;

// builtin.c
int			validate_builtin(t_data *data, t_tree *node);

//executor.c
int			execute(t_data *data);
int			execute_stack(t_data *data, t_stack **stack);
int			execute_cmd(t_data *data, t_stack **stack);
int			execute_pipe(t_data *data, t_stack **stack);
int			execute_cmd_entered(t_data *data, t_stack **stack);
int			execute_cmd_done(t_data **data, t_stack **stack);
int			dummy_heredoc(t_stack **stack);


//stack.c
t_stack		*create_stack(t_data *data);
void		push_stack(t_stack **stack, t_tree *node, int in_fd, int out_fd, t_data *data);
void		pop(t_stack **stack);
t_stack		**get_first_pipe(t_stack **stack);
void		print_stack(t_stack *stack);
void		print_top(t_stack *stack);
t_stack 	**get_next_pipe(t_stack **stack);
void		close_all_pipe_ends(t_stack **stack);
int			stack_size(t_stack *stack);
t_stack		**get_first_log_operator(t_stack **stack);
int		setup_next_to_top(t_data **data, t_stack **stack);
t_stack		**get_first_subshell(t_stack **stack);
int			has_node_type_ancestor(t_stack *stack, t_node_type type);
t_stack 	**get_next_pipe_in_subshell(t_stack **stack);

//child.c
void		child(t_data *data, t_stack **stack);
void		child_redir_in(t_data *data, t_stack **stack, char *cmd);
void		child_no_redir(t_data *data, t_stack **stack, char *cmd);
void		child_redir_out(t_data *data, t_stack **stack, char *cmd);
void		child_heredoc(t_data *data, t_stack **stack, char *cmd);
void		clean_execve_failure(t_data *data, t_stack **stack, char *cmd);

//executor_utils.c
char		*correct_path(t_data * data, t_stack **stack,char *cmd);
char		*run_curr_dir(t_data *data, t_stack **stack, char *cmd);
void		check_for_variables(t_data *data, t_stack **stack);
void		executor_child_errno(t_data *data, t_stack **stack, char *cmd);

//parent.c
int			parent(t_stack **stack, pid_t pid);
int			parent_single_command(t_stack **stack, pid_t pid);
int			parent_heredoc(t_stack **stack, pid_t pid);
int			heredoc_loop(t_stack **stack, char *heredoc, char *res);

//executor_pipe.c
int			execute_pipe_entered(t_data *data, t_stack **stack);
int			execute_pipe_launch_left(t_data *data, t_stack **stack);
int			execute_pipe_launch_right(t_data *data, t_stack **stack);
int			execute_pipe_wait(t_stack **stack);
int			execute_pipe_done(t_data **data, t_stack **stack);

//executor_and.c
int			execute_and(t_data *data, t_stack **stack);
int			execute_and_entered(t_data *data, t_stack **stack);
int			execute_and_launch_left(t_data *data, t_stack **stack);
int			execute_and_launch_right(t_data *data, t_stack **stack);
int			execute_and_done(t_data **data, t_stack **stack);

//executor_or.c
int			execute_or(t_data *data, t_stack **stack);
int			execute_or_entered(t_data *data, t_stack **stack);
int			execute_or_launch_left(t_data *data, t_stack **stack);
int			execute_or_launch_right(t_data *data, t_stack **stack);
int			execute_or_done(t_data **data, t_stack **stack);

//executor_builtin.c
int			execute_builtin(t_data *data, t_stack **stack);
int			execute_builtin_entered(t_data *data, t_stack **stack);
int			execute_builtin_done(t_data **data, t_stack **stack);
int			choose_and_execute_builtin(t_data *data, t_stack **stack);

//executor_echo
int			execute_echo(t_data *data, t_stack **stack);
int			execute_echo_option(t_data *data, t_stack **stack);
int			execute_echo_no_option(t_data *data, t_stack **stack);
int			validate_write(t_data *data, t_stack **stack, int write_res);
int			is_echo_option(char *opt);

//executor_subshell
int			execute_subshell(t_data *data, t_stack **stack);
int			execute_subshell_entered(t_data **data, t_stack **stack);
int			execute_subshell_done(t_data **data, t_stack **stack);

//get_next_line.c
char	*get_next_line(int fd);

//get_next_line_utils.c
int			ft_find_init_nl(char *buff);
char		*ft_handle_new_line(char *buff, char *result, int i);
char		*ft_process_buffer(char *buff, int i);
char		*get_next_line_cont(int fd, char *buffer, char *result, int bytes_read);
char		*ft_gnl_realloc(char *result, int i, int *r);

//executor_env.c
int			execute_env(t_data *data, t_stack **stack);

//executor_cd.c
int			execute_cd(t_data *data, t_stack **stack);
int			cd_fail(char *dir);

//executor_pwd.c
int			execute_pwd(t_data *data, t_stack **stack);

//executor_exit.c
int			execute_exit(t_data *data, t_stack **stack);

//executor_unset.c
int			execute_unset(t_data *data, t_stack **stack);
int			check_unset_input(t_stack **stack);

//executor_error.c
int			validate_fork(t_data *data, t_stack **stack);
void		print_fork_err_mess(void);
void		print_pipe_err_mess(void);
int			validate_pipe(int pipe_res, t_stack **stack);

#endif
