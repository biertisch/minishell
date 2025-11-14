/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:10 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/14 11:41:11 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig;

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc != 1)
		internal_error(INT_ERR_5, NULL, NULL);
	ft_bzero(&data, sizeof(data));
	setup_signals(&data);
	envp_to_list(&data, envp, argv);
	read_input(&data);
	free_all(&data);
	return (data.exit_status);
}
