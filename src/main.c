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

//TESTING
/* static void	print_env_list(t_env *head)
{
	printf("TESTING ENVP_TO_LIST()\n\n");

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
	printf ("TESTING ENV_LIST_TO_ARRAY()\n\n");
	for (int i = 0; env[i]; i++)
	{
		printf("STRING %i\n", i);
		printf("%s\n\n", env[i]);
	}
} */

/* static void	print_lexer_list(t_token *head)
{
	printf("TESTING LEXER()\n\n");
	
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
//END OF TESTING

static void	readline_loop(t_data *data)
{
	while (1)
	{
		data->input = readline("minishell$ ");
		if (!data->input)
			break ;
		if (data->input[0])
		{
			add_history(data->input);
			lexer(data);
			//parser(data);
			//expand(data);
			//execute(data); //TODO PEDRO
		}
		free_command_data(data);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	ft_bzero(&data, sizeof(t_data));
	(void)argc;
	(void)argv;
	//setup_signals //TODO PEDRO
	envp_to_list(&data, envp);
	readline_loop(&data);
	//free_all(&data);
	return (0);
}
