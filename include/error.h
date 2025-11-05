/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 12:29:00 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/05 21:05:12 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

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

# define INT_ERR_0 "invalid environment variable"
# define INT_ERR_1 "ambiguous redirect"
# define INT_ERR_2 "warning: here-document delimited by \
end-of-file (wanted "
# define INT_ERR_3 "invalid option"
# define INT_ERR_4 "too many arguments"
# define INT_ERR_5 "No such file or directory"

//error.c
int		system_error(t_data *data, char *function);
int		syntax_error(t_data *data, char *desc, char *token);
int		internal_error(t_data *data, char *desc, char *cmd, char *arg);
void	error_exit(t_data *data, t_stack **stack);

void	append_postfix(char *msg, char *label);
void	append_prefix(char *msg, char *label);

void	validate_malloc(t_data *data, void *ptr, void *to_free);
void	validate_malloc_env(t_data *data, void *ptr, t_env *node);
void	check_for_errors(int status, t_data *data, t_stack *stack,
			char *command_name);
void	validate_malloc_execute(t_data *data, t_stack **stack, void *ptr,
			void *to_free);

//error_expander.c
int		handle_malloc_failure_expansion(t_data *data, char **argv,
			t_metadata *info, int argc);
int		handle_wildcard_rebuild_failure(char **argv, t_metadata *info,
			int argc);
void	validate_malloc_wildcard(t_data *data, void *ptr, t_list *node);

//error_parser.c
int		check_unsupported_syntax(t_data *data, char *input);
void	validate_malloc_parser(t_data *data, void *ptr, t_tree *left,
			t_tree *right);

#endif