/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 12:29:00 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/08 11:47:44 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "minishell.h"

# define ERR_BUFFER_SIZE 512

# define SYN_ERR_0 "syntax error: missing matching"
# define SYN_ERR_1 "command separator ';' not supported"
# define SYN_ERR_2 "arithmetic operations not supported"
# define SYN_ERR_3 "arithmetic expansion not supported"
# define SYN_ERR_4 "command substitution not supported"
# define SYN_ERR_5 "syntax error near unexpected token"
# define SYN_ERR_6 "unexpected EOF while looking for matching"
# define SYN_ERR_7 "syntax error: unexpected end of file"
# define SYN_ERR_8 "warning: here-document delimited by \
end-of-file (wanted "

# define INT_ERR_0 "invalid environment variable"
# define INT_ERR_1 "ambiguous redirect"
# define INT_ERR_2 "invalid option"
# define INT_ERR_3 "too many arguments"
# define INT_ERR_4 "No such file or directory"
# define INT_ERR_5 "arguments not supported :("
# define INT_ERR_6 "command not found"
# define INT_ERR_7 "cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory"
# define INT_ERR_8 "numeric argument required"

//error.c
int		syntax_error(t_data *data, char *desc, char *token);
int		internal_error(char *desc, char *cmd, char *arg);
int		system_error(char *desc, char *function);
void	error_exit(t_data *data, t_stack **stack);
void	validate_malloc(t_data *data, void *ptr, void *to_free);

//error_executor.c
void	validate_malloc_execute(t_data *data, t_stack **stack, void *ptr,
			void *to_free);

//error_expander.c
int		handle_malloc_failure_expansion(t_data *data, char **argv,
			t_metadata *info, int argc);
int		handle_wildcard_rebuild_failure(char **argv, t_metadata *info,
			int argc);
void	validate_malloc_wildcard(t_data *data, void *ptr, t_list *node);

//error_parser.c
int		missing_quote(t_data *data, char quote);
int		check_unsupported_syntax(t_data *data, char *input);
void	validate_malloc_parser(t_data *data, void *ptr, t_tree *left,
			t_tree *right);

//error_utils.c
void	append_postfix(char *msg, char *label);
void	append_prefix(char *msg, char *label);
void	validate_malloc_env(t_data *data, void *ptr, t_env *node);

#endif
