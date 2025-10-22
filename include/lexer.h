/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 23:24:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/21 23:25:09 by beatde-a         ###   ########.fr       */
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
int			lexer(t_data *data);

//lexer_list.c
void		free_lexer_list(t_token **head);
void		free_lexer_node(t_token **node);
t_token		*get_last_lexer_node(t_token *head);
void		add_lexer_node(t_token **head, t_token *new_node);
t_token		*create_lexer_node(t_token_type type, char *value);

//lexer_utils.c
int			is_fd(char *input);
int			is_quote(char c);
int			is_operator(char *s);
int			get_operator_len(char *s);

#endif