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
// static void	print_env_list(t_env *head)
// {
// 	printf("-----TESTING ENVP_TO_LIST-----\n\n");

// 	for (int i = 0; head; i++)
// 	{
// 		printf("NODE %i\n", i);
// 		if (head->key)
// 			printf("%s", head->key);
// 		if (head->value)
// 			printf("=%s", head->value);
// 		printf("\n\n");
// 		head = head->next;
// 	}
// }

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
		if (head->type == WORD)
			printf("Type: WORD\n");
		else if (head->type == PIPE)
			printf("Type: PIPE\n");
		else if (head->type == AND)
			printf("Type: AND\n");
		else if (head->type == OR)
			printf("Type: OR\n");
		else if (head->type == REDIR_IN)
			printf("Type: REDIR_IN\n");
		else if (head->type == REDIR_OUT)
			printf("Type: REDIR_OUT\n");
		else if (head->type == APPEND)
			printf("Type: APPEND\n");
		else if (head->type == HEREDOC)
			printf("Type: HEREDOC\n");
		else if (head->type == FD)
			printf("Type: FD\n");
		else if (head->type == LPAREN)
			printf("Type: LPAREN\n");
		else if (head->type == RPAREN)
			printf("Type: RPAREN\n");
		if (head->value)
			printf("Value: %s\n", head->value);
		printf("\n");
		head = head->next;
	}
} */
//-----------------PARSER-----------------
// static void print_parser_node(t_tree *node, int depth, char *pos)
// {
// 	if (!node)
// 		return ;

// 	printf("NODE %i %s\n", depth, pos);
// 	if (node->type == NODE_CMD)
// 		printf("Type: NODE_CMD\n");
// 	else if (node->type == NODE_PIPE)
// 		printf("Type: NODE_PIPE\n");
// 	else if (node->type == NODE_AND)
// 		printf("Type: NODE_AND\n");
// 	else if (node->type == NODE_OR)
// 		printf("Type: NODE_OR\n");
// 	else if (node->type == NODE_SUBSHELL)
// 		printf("Type: NODE_SUBSHELL\n");
// 	if (node->argv)
// 		for (int i = 0; node->argv[i]; i++)
// 			printf("Command argv[%i] = %s\n", i, node->argv[i]);
// 	t_redir *redir = node->redir;
// 	while (redir)
// 	{
// 		printf("Redir: \n");
// 		if (redir->type == REDIR_IN)
// 			printf("Type: REDIR_IN\n");
// 		else if (redir->type == REDIR_OUT)
// 			printf("Type: REDIR_OUT\n");
// 		else if (redir->type == APPEND)
// 			printf("Type: APPEND\n");
// 		else if (redir->type == HEREDOC)
// 			printf("Type: HEREDOC\n");
// 		printf("FD: %d\n", redir->fd);
// 		printf("File: %s\n", redir->file);
// 		redir = redir->next;
// 	}
// 	printf("\n");
// 	print_parser_node(node->left, depth + 1, "left");
// 	print_parser_node(node->right, depth + 1, "right");
// }

// static void print_parser_tree(t_tree *head)
// {
// 	printf("-----TESTING PARSER-----\n\n");
// 	print_parser_node(head, 0, "head");

// }
//-----------------END OF TESTING-----------------

static void	process_input(t_data *data)
{
	add_history(data->input);
	if (lexer(data))
		return ;
	if (parser(data))
		return ;
	if (expand(data, data->parser_tree))
		return ;
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
