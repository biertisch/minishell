/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 13:13:49 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/02 13:13:52 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

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
}	t_stack;

// builtin.c
int			is_builtin(char *cmd);
int			is_builtin_no_fork(char *cmd);
int			validate_builtin(t_data *data, t_tree *node, int i);
int			validate_builtin_flags(t_data *data, char **argv, char *allowed);
int			validate_env(t_data *data, char **argv);
int			print_builtin_usage(char *cmd);

//executor.c
int			execute(t_data *data);
int			execute_stack(t_data *data, t_stack **stack);
int			execute_cmd(t_data *data, t_stack **stack);
int			execute_pipe(t_data *data, t_stack **stack);
int			execute_cmd_entered(t_data *data, t_stack **stack);
int			execute_cmd_done(t_data **data, t_stack **stack);

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
void		close_all_open_redir_ends(t_data *data);

//child.c
void		child(t_data *data, t_stack **stack);
void		child_redir_in(t_data *data, t_stack **stack, char *cmd, t_redir *redir);
void		child_redir_out(t_data *data, t_stack **stack, char *cmd, t_redir *redir);
void		child_heredoc(t_data *data, t_stack **stack, char *cmd, t_redir *redir);
void		child_no_redir(t_data *data, t_stack **stack, char *cmd);
void		clean_execve_failure(t_data *data, t_stack **stack, char *cmd);
void	check_no_cmd(t_data *data, t_stack **stack);
void		handle_redirects(t_data *data, t_stack **stack, char *cmd, t_redir *redir);


//executor_utils.c
char		*correct_path(t_data * data, t_stack **stack,char *cmd);
char		*run_curr_dir(t_data *data, t_stack **stack, char *cmd);
void		check_for_variables(t_data *data, t_stack **stack);
void		executor_child_errno(t_data *data, t_stack **stack, char *cmd);
void		executor_cleanup(t_data *data, t_stack **stack, char *cmd);
char		**split_by_first_equal(char *var);
void		cmd_not_found(t_data *data, t_stack **stack, char **paths, char *slash_path);
void		cmd_is_directory(t_data *data, t_stack **stack, int fd);
void		check_err_output(t_data *data, t_stack **stack, char **paths, char *slash_path);

//parent.c
int			parent(t_stack **stack, pid_t pid);
int			parent_single_command(t_stack **stack, pid_t pid);
int			parent_heredoc(t_stack **stack, pid_t pid);

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
int			execute_builtin_should_run_child(t_data *data, t_stack **stack, int cmd_i);
int			execute_builtin_done(t_data **data, t_stack **stack);
int			choose_and_execute_builtin(t_data *data, t_stack **stack);

//executor_echo
int			execute_echo(t_data *data, t_stack **stack);
int			execute_echo_option(t_data *data, t_stack **stack, int cmd_i);
int			execute_echo_no_option(t_data *data, t_stack **stack, int cmd_i);
int			is_echo_option(char *opt);

//executor_subshell
int			execute_subshell(t_data *data, t_stack **stack);
int			execute_subshell_entered(t_data **data, t_stack **stack);
int			execute_subshell_done(t_data **data, t_stack **stack);
int			subshell_redir(t_data **data, t_stack **stack);
int		check_redir_in_subshell(t_stack **stack);

//executor_env.c
int			execute_env(t_data *data, t_stack **stack);

//executor_cd.c
int			execute_cd(t_data *data, t_stack **stack);
int			execute_cd_option(t_data *data, t_stack **stack, int cmd_i, int *chdir_res);
int			cd_fail(char *dir);

//executor_pwd.c
int			execute_pwd(t_data *data, t_stack **stack);

//executor_exit.c
int			execute_exit(t_data *data, t_stack **stack);
void		check_exit_input(t_data *data, t_stack **stack, int *exit_code, int cmd_i);

//executor_unset.c
int			execute_unset(t_data *data, t_stack **stack);
int			check_unset_input(t_stack **stack);

//executor_error.c
int			validate_fork(t_data *data, t_stack **stack);
void		print_fork_err_mess(void);
void		print_pipe_err_mess(void);
int			validate_pipe(int pipe_res, t_stack **stack);

//executor_export.c
int			execute_export(t_data *data, t_stack **stack);
int			execute_export_val_found(t_data *data, t_stack **stack, char **kv_split, t_env **env);
int			execute_export_option(t_data *data, t_stack **stack, int cmd_i);
int			execute_export_val_found(t_data *data, t_stack **stack, char **kv_split, t_env **env);
void			execute_export_invalid_var(t_stack **stack, int cmd_i);
int			execute_export_val_not_found(t_data *data, t_stack **stack, char **kv_split);
int			execute_export_no_option(t_data *data, t_stack **stack);

//executor_sort_env.c
void			sort_env(t_data **data);

//variable_utils.c
int			cmd_has_variable(t_data *data, t_stack **stack);
int			get_first_command(t_data *data, t_stack **stack);
int			has_command(t_data *data, t_stack **stack);
int			variable_key_not_found(t_data *data, t_stack **stack, char **kv_split);

//executor_heredoc.c
int			check_for_heredoc(t_data *data);
int			check_heredoc_right(t_data *data);
int			check_heredoc_left(t_data *data);
int			execute_heredoc(t_data *data, t_redir *redir);
t_redir		*get_last_heredoc(t_redir *redir);

//executor_redirect.c
int			traverse_redir_in(t_data *data, t_stack **stack);
int			open_redir_in(t_data *data, t_redir *redir);
int			close_redir_in(t_redir *redir);
void		handle_open_errors(t_redir *redir);
int			check_redir_in_left(t_data *data, t_stack **stack);
int			check_redir_in_right(t_data *data, t_stack **stack);
int			push_left_until_cmd_redir(t_data *data, t_stack **stack);

//executor_heredoc2.c
int			run_heredoc_child(t_data *data, t_redir *redir);
int			heredoc(t_data *data, t_redir *redir);
int 		run_heredoc_parent(t_data *data, t_redir *redir, pid_t pid);
int			copy_heredoc_input(t_data *data, t_redir *redir);
int			wait_for_heredoc(t_data *data, pid_t pid);

//executo_builtins_utils.c
int			*duplicate_std(void);
void			undo_duplicate_std(void);
void		execute_builtin_check_for_pipe(t_data *data, t_stack **stack);

#endif
