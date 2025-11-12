/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 13:13:49 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/07 23:38:44 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

//builtin.c
int				is_builtin(char *cmd);
int				is_builtin_no_fork(char *cmd);
int				validate_builtin(t_data *data, t_tree *node, int i);
int				validate_env(t_data *data, char **argv);
int				print_builtin_usage(char *cmd);
int				has_builtin_flag(char **argv);

//executor.c            	
int				execute(t_data *data);
int				execute_stack(t_data *data, t_stack **stack);
int				execute_cmd(t_data *data, t_stack **stack);
int				execute_pipe(t_data *data, t_stack **stack);
int				execute_cmd_entered(t_data *data, t_stack **stack);
int				execute_cmd_done(t_data **data, t_stack **stack);
int				execute_cmd_no_cmd(t_data *data, t_stack **stack);
int				execute_cmd_pre_processing(t_data *data, t_stack **stack);

//stack.c               	
int				stack_size(t_stack *stack);
int				setup_next_to_top(t_data **data, t_stack **stack);
int				has_node_type_ancestor(t_stack *stack, t_node_type type);
void			push_stack(t_stack **stack, t_tree *node,
					t_fd_pair fds, t_data *data);
void			pop(t_stack **stack);
void			print_stack(t_stack *stack);
void			print_top(t_stack *stack);
void			close_all_pipe_ends(t_stack **stack);
void			close_all_open_redir_ends(t_data *data);
t_stack			*create_stack(t_data *data);
t_stack			**get_first_pipe(t_stack **stack);
t_stack			**get_next_pipe(t_stack **stack);
t_stack			**get_first_log_operator(t_stack **stack);
t_stack			**get_first_subshell(t_stack **stack);
t_stack			**get_next_pipe_in_subshell(t_stack **stack);
t_fd_pair		get_fd_pair(int in_fd, int out_fd);

//child.c       	
int				has_failed_redirect(t_redir *redir);
void			child(t_data *data, t_stack **stack);
void			child_redir_in(t_data *data, t_stack **stack,
					char *cmd, t_redir *redir);
void			child_redir_out(t_data *data, t_stack **stack,
					char *cmd, t_redir *redir);
void			child_heredoc(t_data *data, t_stack **stack,
					char *cmd, t_redir *redir);
void			child_no_redir(t_data *data, t_stack **stack, char *cmd);
void			clean_execve_failure(t_data *data, t_stack **stack);
void			check_no_cmd(t_data *data, t_stack **stack);
void			handle_redirects(t_data *data, t_stack **stack,
					char *cmd, t_redir *redir);
void			child_execute(t_data *data, t_stack **stack, char *cmd);

//executor_utils.c
char			*correct_path(t_data *data, t_stack **stack, char *cmd);
char			*run_curr_dir(t_data *data, t_stack **stack, char *cmd);
char			**split_by_first_equal(char *var);
char			*cmd_not_found(t_data *data, t_stack **stack,
					char **paths, char *slash_path);
char			**get_path_split(t_data *data, t_stack **stack,
					char *slash_path);
char			*get_slash_path(t_data *data, t_stack **stack, char *cmd);
void			check_for_variables(t_data *data, t_stack **stack);
void			executor_child_errno(t_data *data, t_stack **stack,
					char *to_free, char **also_to_free);
void			executor_cleanup(t_data *data, t_stack **stack, char *cmd);
void			cmd_is_directory(t_data *data, t_stack **stack, int fd);
void			check_err_output(t_data *data, t_stack **stack,
					char **paths, char *slash_path);

//parent.c
int				parent(t_stack **stack, pid_t pid);
int				parent_single_command(t_stack **stack, pid_t pid);
int				parent_heredoc(t_stack **stack, pid_t pid);

//executor_pipe.c     	
int				execute_pipe_entered(t_data *data, t_stack **stack);
int				execute_pipe_launch_left(t_data *data, t_stack **stack);
int				execute_pipe_launch_right(t_stack **stack);
int				execute_pipe_wait(t_stack **stack);
int				execute_pipe_done(t_data **data, t_stack **stack);

//executor_and.c
int				execute_and(t_data *data, t_stack **stack);
int				execute_and_entered(t_data *data, t_stack **stack);
int				execute_and_launch_left(t_data *data, t_stack **stack);
int				execute_and_launch_right(t_stack **stack);
int				execute_and_done(t_data **data, t_stack **stack);

//executor_or.c       	
int				execute_or(t_data *data, t_stack **stack);
int				execute_or_entered(t_data *data, t_stack **stack);
int				execute_or_launch_left(t_data *data, t_stack **stack);
int				execute_or_launch_right(t_stack **stack);
int				execute_or_done(t_data **data, t_stack **stack);

//executor_builtin.c    	
int				execute_builtin(t_data *data, t_stack **stack);
int				execute_builtin_entered(t_data *data, t_stack **stack);
int				execute_builtin_should_run_child(t_data *data,
					t_stack **stack, int cmd_i);
int				execute_builtin_done(t_data **data, t_stack **stack);
int				choose_and_execute_builtin(t_data *data, t_stack **stack,
					int cmd_i);

//executor_echo       	
int				execute_echo(t_data *data, t_stack **stack);
int				execute_echo_option(t_data *data, t_stack **stack, int cmd_i);
int				execute_echo_no_option(t_data *data, t_stack **stack,
					int cmd_i);
int				is_echo_option(char *opt);

//executor_subshell   	
int				execute_subshell(t_data *data, t_stack **stack);
int				execute_subshell_entered(t_data **data, t_stack **stack);
int				execute_subshell_done(t_data **data, t_stack **stack);
int				subshell_redir(t_stack **stack, t_redir *redir);
int				check_redir_in_subshell(t_stack **stack);
void			execute_subshell_entered_parent(t_stack **stack, pid_t pid);

//executor_env.c
int				execute_env(t_data *data, t_stack **stack);
int				is_env_option(t_stack **stack, int cmd_i);
void			print_env_args(t_stack **stack,
					int exit_status, int cmd_i);
void			pre_print_env_args(t_stack **stack,
					int exit_status, int cmd_i);

//executor_cd.c
int				execute_cd(t_data *data, t_stack **stack);
int				execute_cd_option(t_data *data, t_stack **stack, int cmd_i);
int				cd_fail(t_data *data, t_stack **stack, char *dir);
int				execute_cd_continue(t_data *data, t_stack **stack,
					char *curr_pwd);

//executor_pwd.c      	
int				execute_pwd(t_data *data, t_stack **stack);

//executor_exit.c       	
int				execute_exit(t_data *data, t_stack **stack);
void			check_exit_input(t_data *data, t_stack **stack,
					int *exit_code, int cmd_i);

//executor_unset.c
int				execute_unset(t_data *data, t_stack **stack);
int				check_unset_input(t_stack **stack);
int				found_victim(t_data *data, t_unset_vars *v);
t_unset_vars	get_begginer_u_v(t_data *data, int i);

//executor_error.c
int				validate_fork(t_data *data, t_stack **stack);
int				validate_pipe(int pipe_res, t_stack **stack);
void			print_fork_err_mess(void);
void			print_pipe_err_mess(void);

//executor_export.c
int				execute_export(t_data *data, t_stack **stack);
int				execute_export_val_found(t_data *data,
					t_stack **stack, char **kv_split, t_env **env);
int				execute_export_option(t_data *data, t_stack **stack, int cmd_i);
int				execute_export_val_found(t_data *data,
					t_stack **stack, char **kv_split, t_env **env);
int				execute_export_val_not_found(t_data *data,
					t_stack **stack, char **kv_split);
int				execute_export_no_option(t_data *data);
void			execute_export_invalid_var(t_stack **stack, int cmd_i);
void			execute_export_handle_underscore(t_data *data, t_stack **stack);

//executor_sort_env.c
void			sort_env(t_data **data);

//variable_utils.c
int				cmd_has_variable(t_data *data, t_stack **stack);
int				get_first_command(t_data *data, t_stack **stack);
int				has_command(t_data *data, t_stack **stack);
int				variable_key_not_found(t_data *data, t_stack **stack,
					char **kv_split);
void			cmd_has_valid_variable(t_data *data, t_stack **stack,
					char **kv_split);

//executor_redirect.c
int				traverse_redir_in(t_data *data, t_stack **stack);
int				open_redir(t_data *data, t_redir *redir);
int				close_redir_in(t_redir *redir);
int				check_redir_in_left(t_data *data, t_stack **stack);
int				check_redir_in_right(t_data *data, t_stack **stack);
int				push_left_until_cmd_redir(t_data *data, t_stack **stack);
int				open_redir_out_ok(t_data *data, t_redir *redir,
					t_list *new, int *fd);
int				open_redir_out(t_data *data, t_redir *redir,
					t_list *new, int *fd);
int				open_redir_in(t_data *data, t_redir *redir,
					t_list *new, int *fd);

//executo_builtins_utils.c
int				*duplicate_std(void);
void			undo_duplicate_std(int dup);
void			execute_builtin_check_for_pipe(t_data *data, t_stack **stack);

#endif
