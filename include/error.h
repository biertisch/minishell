/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 12:29:00 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/05 12:38:24 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "minishell.h"

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
# define ERR_10 "command separator ';' not supported"
# define ERR_11 "minishell: warning: here-document delimited by \
end-of-file (wanted '"
# define ERR_12 "arithmetic expansion not supported"
# define ERR_13 "command substitution not supported"

//error.c
int		system_error(t_data *data, char *function);
int		syntax_error(t_data *data, char *desc, char *token);
int		internal_error(t_data *data, char *desc, char *cmd, char *arg);
void	error_exit(t_data *data, t_stack **stack);
void	validate_malloc(t_data *data, void *ptr, void *to_free);
void	validate_malloc_tree(t_data *data, void *ptr, t_tree *left,
			t_tree *right);
void	validate_malloc_env(t_data *data, void *ptr, t_env *node);
void	check_for_errors(int status, t_data *data, t_stack *stack,
			char *command_name);
void	validate_malloc_wildcard(t_data *data, void *ptr, t_list *node,
			char **new_argv);
void	validate_malloc_execute(t_data *data, t_stack **stack, void *ptr,
			void *to_free);
void	validate_malloc_unfinished_array(t_data *data, void *ptr, char **arr,
			int size);

//error_expander.c
int		handle_malloc_failure_expansion(t_data *data, char **argv,
			t_metadata *info, int argc);
int		handle_wildcard_rebuild_failure(char **argv, t_metadata *info,
			int argc);

//error_parser.c
int		check_unsupported_syntax(t_data *data, char *input);

#endif