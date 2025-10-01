/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:10 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/22 10:22:59 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_sig_received = 0;

//accept arguments or issue warning?
int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	ft_bzero(&data, sizeof(t_data));
	setup_signals(&data);
	envp_to_list(&data, envp, argv);
	prompt_input(&data);
	free_all(&data);
	return (EXIT_SUCCESS);
}
