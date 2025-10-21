/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_def.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:16:36 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/21 15:47:31 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_DEF_H
# define STRUCT_DEF_H

typedef struct s_env	t_env;
typedef struct s_data	t_data;
typedef struct s_stack	t_stack;
typedef struct s_token	t_token;
typedef struct s_redir	t_redir;
typedef struct s_tree	t_tree;

typedef enum e_token_type
{
	WORD,
	PIPE,
	AND,
	OR,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	FD,
	LPAREN,
	RPAREN
}	t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;


typedef enum e_phase
{
	ENTERED,
	LAUNCH_LEFT,
	LAUNCH_RIGHT,
	WAIT,
	DONE
}	t_phase;

typedef enum e_error
{
	SYSTEM_ERR,
	SYNTAX_ERR,
	INTERNAL_ERR
}	t_error;



#endif
