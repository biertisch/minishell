/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:26:29 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/01 19:37:00 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arg_info	*init_argv_info(t_data *data, char **argv)
{
	int			argc;
	t_arg_info	*argv_info;

	if (!argv || !*argv)
		return (NULL);
	argc = get_argc(argv);
	if (argc == 0)
		return (NULL);
	argv_info = ft_calloc(sizeof(t_arg_info), argc);
	validate_malloc(data, argv_info, NULL);
	return (argv_info);
}
