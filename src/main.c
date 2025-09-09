/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:10 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/09 15:24:33 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	ft_bzero(&data, sizeof(t_data));
	(void)argc;
	(void)argv;
	//setup_signals //TODO BEATRIZ
	envp_to_list(&data, envp);
	prompt_input(&data);
	free_all(&data);
	return (EXIT_SUCCESS);
}

// TODO BEATRIZ
// - incomplete input
// - handle empty envp
// - setup signals
// - check recursion