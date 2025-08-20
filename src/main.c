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

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	ft_bzero(&data, sizeof(t_data));
	(void)argc;
	(void)argv;
	//setup_signals --- PEDRO
	envp_to_list(&data, envp);
	while (1)
	{
		data.prompt = readline("Command: ");
		if (!data.prompt)
			break ;
		if (data.prompt[0])
		{
			add_history(data.prompt);
			//lexer(&data);
			//parser(&data);
			//expand(&data);
			//execute(&data);
		}
		free(data.prompt);
		data.prompt = NULL;
	}
	//cleanup(&data);
	return (0);
}
