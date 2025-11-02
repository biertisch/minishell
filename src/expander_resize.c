/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_resize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:23:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/02 19:09:21 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_argc(char **argv)
{
	int	i;

	if (!argv || !*argv)
		return (0);
	i = 0;
	while (argv[i])
		i++;
	return (i);
}

// int	get_expanded_argc(char **argv, char	**raw_argv)
// {
// 	int	count;
// 	int	i;

// 	if (!argv || !*argv || !raw_argv)
// 		return (0);
// 	count = get_argc(argv);
// 	i = 0;
// 	while (argv[i])
// 	{
// 		if (!*(argv[i]) && *(raw_argv[i]))
// 			count--;
// 		else if ()
// 		i++;
// 	}

// }

// int	resize_argv(t_data *data, char ***argv, char **raw_argv)
// {
// 	//get new argc
// 	//allocate new argv
// 	//if argv[i] == raw_argv[i] (minus quotes), simple strdup
// 	//else if argv[i] is empty remove it
// 	//else if argv[i] has IFS (space, \n, \t) and was not within quotes, split
// }
