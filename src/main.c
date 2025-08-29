/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:10 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/20 10:38:10 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//----------------TESTING----------------
//------------------ENVP-----------------
/* static void	print_env_list(t_env *head)
{
	printf("-----TESTING ENVP_TO_LIST-----\n\n");

	for (int i = 0; head; i++)
	{
		printf("NODE %i\n", i);
		if (head->key)
			printf("%s", head->key);
		if (head->value)
			printf("=%s", head->value);
		printf("\n\n");
		head = head->next;
	}
} */

/* static void	print_env_array(char **env)
{
	printf ("-----TESTING ENV_LIST_TO_ARRAY-----\n\n");
	for (int i = 0; env[i]; i++)
	{
		printf("STRING %i\n", i);
		printf("%s\n\n", env[i]);
	}
} */
//-----------------LEXER-----------------
/* static void	print_lexer_list(t_token *head)
{
	printf("-----TESTING LEXER-----\n\n");
	
	for (int i = 0; head; i++)
	{
		printf("NODE %i\n", i);
		if (head->type == TOKEN_WORD)
			printf("Type: TOKEN_WORD\n");
		else if (head->type == TOKEN_PIPE)
			printf("Type: TOKEN_PIPE\n");
		else if (head->type == TOKEN_AND_IF)
			printf("Type: TOKEN_AND_IF\n");
		else if (head->type == TOKEN_OR_IF)
			printf("Type: TOKEN_OR_IF\n");
		else if (head->type == TOKEN_REDIR_IN)
			printf("Type: TOKEN_REDIR_IN\n");
		else if (head->type == TOKEN_REDIR_OUT)
			printf("Type: TOKEN_REDIR_OUT\n");
		else if (head->type == TOKEN_APPEND)
			printf("Type: TOKEN_APPEND\n");
		else if (head->type == TOKEN_HEREDOC)
			printf("Type: TOKEN_HEREDOC\n");
		else if (head->type == TOKEN_FD)
			printf("Type: TOKEN_FD\n");
		else if (head->type == TOKEN_LPAREN)
			printf("Type: TOKEN_LPAREN\n");
		else if (head->type == TOKEN_RPAREN)
			printf("Type: TOKEN_RPAREN\n");
		if (head->value)
			printf("Value: %s\n", head->value);
		printf("\n");
		head = head->next;
	}
} */
//-----------------PARSER-----------------
/* static void print_parser_node(t_ast *node, int depth, char *pos)
{
	if (!node)
		return ;

	printf("NODE %i %s\n", depth, pos);
	if (node->type == NODE_CMD)
		printf("Type: NODE_CMD\n");
	else if (node->type == NODE_PIPE)
		printf("Type: NODE_PIPE\n");
	else if (node->type == NODE_AND_IF)
		printf("Type: NODE_AND_IF\n");
	else if (node->type == NODE_OR_IF)
		printf("Type: NODE_OR_IF\n");
	else if (node->type == NODE_SUBSHELL)
		printf("Type: NODE_SUBSHELL\n");
	if (node->cmd)
	{
		if (node->cmd->argv)
			for (int i = 0; node->cmd->argv[i]; i++)
				printf("Command argv[%i] = %s\n", i, node->cmd->argv[i]);
		t_redir *redir = node->cmd->redirs;
		while (redir)
		{
			printf("Redir: \n");
			if (redir->type == TOKEN_REDIR_IN)
				printf("Type: TOKEN_REDIR_IN\n");
			else if (redir->type == TOKEN_REDIR_OUT)
				printf("Type: TOKEN_REDIR_OUT\n");
			else if (redir->type == TOKEN_APPEND)
				printf("Type: TOKEN_APPEND\n");
			else if (redir->type == TOKEN_HEREDOC)
				printf("Type: TOKEN_HEREDOC\n");
			printf("FD: %d\n", redir->fd);
			printf("File: %s\n", redir->file);
			redir = redir->next;
		}
	}
	printf("\n");
	print_parser_node(node->left, depth + 1, "left");
	print_parser_node(node->right, depth + 1, "right");
}

static void print_parser_list(t_ast *head)
{
	printf("-----TESTING PARSER-----\n\n");
	print_parser_node(head, 0, "head");

} */
//-----------------END OF TESTING-----------------

static void	process_input(t_data *data)
{
	add_history(data->input);
	if (lexer(data))
		return ;
	if (parser(data))
		return ;
	//if (expand(data))
		//return ;
	//if (execute(data)) //TODO PEDRO
		//return ;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	ft_bzero(&data, sizeof(t_data));
	(void)argc;
	(void)argv;
	//setup_signals //TODO PEDRO
	envp_to_list(&data, envp);
	while (1)
	{
		data.input = readline("minishell$ ");
		if (!data.input)
			break ;
		if (data.input[0])
			process_input(&data);
		free_command_data(&data);
	}
	free_all(&data);
	return (EXIT_SUCCESS);
}

//TO THINK ABOUT
//-Which characters are unsupported beyond \ and ;? eg {}[]!:#%
//-How to handle <& and >&?
//-How to handle "echo hello | >file cat"
//-Avoid double error messages