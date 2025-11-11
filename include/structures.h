/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 11:16:36 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/08 12:08:42 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

typedef enum e_outcome
{
	VALID,
	INVALID,
	INCOMPLETE,
	INCOMPLETE_EOF
}	t_outcome;

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

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

typedef struct s_metadata
{
	int				*quote_map;
	int				*expand_map;
	char			*key;
	int				key_len;
	char			*value;
	int				value_len;
	int				total_len;
}	t_metadata;

typedef struct s_redir
{
	t_token_type	type;
	int				fd;
	char			*file;
	char			*heredoc_input;
	t_metadata		info;
	int				in_fd;
	int				out_fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_fd_pair
{
	int	in_fd;
	int	out_fd;
}	t_fd_pair;

typedef struct s_tree
{
	t_node_type		type;
	char			**argv;
	t_metadata		*argv_info;
	t_redir			*redir;
	struct s_tree	*left;
	struct s_tree	*right;
}	t_tree;

typedef struct s_stack
{
	t_phase			phase;
	t_node_type		type;
	t_tree			*node;
	int				in_fd;
	int				out_fd;
	int				pipe[2];
	int				child_count;
	int				exit_status;
	pid_t			child_pid[2];
	struct s_stack	*next;
}	t_stack;

typedef struct s_data
{
	char			*input;
	char			*prompt;
	char			**env;
	t_env			*env_list;
	t_token			*lexer_list;
	t_tree			*parser_tree;
	t_stack			*stack;
	int				exit_status;
	t_list			*open_redirs;
}	t_data;

#endif
