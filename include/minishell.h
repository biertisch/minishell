/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:04:14 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/20 10:04:14 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termcap.h>
# include <termios.h>
# include <unistd.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	AND_IF,
	OR_IF,
	LPAREN,
	RPAREN,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}	t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND_IF,
	NODE_OR_IF,
	NODE_SUBSHELL
}	t_node_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_ast
{
	t_node_type		type;
	void			*content;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char	**argv;
	t_redir	*redirs;
}	t_cmd;

// maybe add struct with int *fd_open and int fd_open_counter

typedef struct s_data
{
	char	*prompt;
	char	**env;
	t_env	*env_list;
	t_token	*token_list;
	t_ast	*ast_list;
}	t_data;

//env_list.c
t_env	*create_env_node(char *key, char *value);
void	add_env_node(t_env **head, t_env *new_node);
t_env	*get_last_node(t_env *head);
void	free_env_node(t_env **node);
void	free_env_list(t_env **head);

//env.c
void	envp_to_list(t_data *data, char **envp);

#endif