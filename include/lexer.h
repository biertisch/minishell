/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:24:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/05 14:53:17 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

//lexer.c
int				lexer(t_data *data, char *input);
t_token_type	get_token_type(char *input);
int				get_token_value(t_data *data, char *input, char **value);
void			add_token(t_data *data, t_token **lexer_list, t_token_type type,
					char *value);
int				toggle_quote(char c, char *quote);

//lexer_list.c
t_token			*create_lexer_node(t_token_type type, char *value);
void			add_lexer_node(t_token **head, t_token *new_node);
t_token			*get_last_lexer_node(t_token *head);
void			free_lexer_node(t_token **node);
void			free_lexer_list(t_token **head);

//lexer_type.c
int				is_operator(char *s);
int				get_operator_len(char *s);
int				is_quote(char c);
int				is_fd(char *input);
int				is_arithmetic_op(char *input);

#endif