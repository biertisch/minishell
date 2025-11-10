/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:10 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/10 13:02:13 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig;

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	ft_bzero(&data, sizeof(data));
	setup_signals(&data);
	envp_to_list(&data, envp, argv);
	read_input(&data);
	free_all(&data);
	return (data.exit_status);
}
