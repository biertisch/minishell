//DELETE WHEN COMPLETE
#include "../include/minishell.h"

//---------------------ENV-----------------------
void	print_env_list(t_env *head)
{
	printf("\n-----TESTING ENVP_TO_LIST-----\n\n");

	if (!head)
	{
		printf("NULL\n\n");
		return ;
	}

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
}

void	print_env_array(char **env)
{
	printf ("\n-----TESTING ENV_LIST_TO_ARRAY-----\n\n");

	if (!env)
	{
		printf("NULL\n\n");
		return ;
	}

	for (int i = 0; env[i]; i++)
	{
		printf("STRING %i\n", i);
		printf("%s\n\n", env[i]);
	}
}

//----------------------LEXER--------------------
void	print_lexer_list(t_token *head)
{
	printf("\n-----TESTING LEXER-----\n\n");

	if (!head)
	{
		printf("NULL\n\n");
		return ;
	}

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
}

//----------------------PARSER-------------------
static void print_parser_node(t_tree *node, int depth, char *pos)
{
	if (!node)
		return ;

	printf("NODE %i %s\n", depth, pos);
	if (node->type == NODE_CMD)
		printf("Type: NODE_CMD\n");
	else if (node->type == NODE_BUILTIN)
		printf("Type: NODE_BUILTIN\n");
	else if (node->type == NODE_PIPE)
		printf("Type: NODE_PIPE\n");
	else if (node->type == NODE_AND)
		printf("Type: NODE_AND\n");
	else if (node->type == NODE_OR)
		printf("Type: NODE_OR\n");
	else if (node->type == NODE_SUBSHELL)
		printf("Type: NODE_SUBSHELL\n");
	if (node->argv)
		for (int i = 0; node->argv[i]; i++)
			printf("Command argv[%i] = %s\n", i, node->argv[i]);
	t_redir *redir = node->redir;
	while (redir)
	{
		printf("Redir: \n");
		if (redir->type == REDIR_IN)
			printf("Type: REDIR_IN\n");
		else if (redir->type == REDIR_OUT)
			printf("Type: REDIR_OUT\n");
		else if (redir->type == APPEND)
			printf("Type: APPEND\n");
		else if (redir->type == HEREDOC)
			printf("Type: HEREDOC\n");
		printf("FD: %d\n", redir->fd);
		printf("File: %s\n", redir->file);
		redir = redir->next;
	}
	printf("\n");
	print_parser_node(node->left, depth + 1, "left");
	print_parser_node(node->right, depth + 1, "right");
}

void	print_parser_tree(t_tree *head)
{
	printf("\n-----TESTING PARSER-----\n\n");
	if (!head)
	{
		printf("NULL\n\n");
		return ;
	}
	print_parser_node(head, 0, "head");
}

//----------------------BUILTIN------------------
void	test_builtin(t_data *data, t_tree *head)
{
	if (!head)
		return ;
	validate_builtin(data, head);
	test_builtin(data, head->left);
	test_builtin(data, head->right);
}
