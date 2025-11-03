/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_resize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:23:17 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/03 14:42:10 by beatde-a         ###   ########.fr       */
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

// int	get_expanded_argc(t_data * data, char **argv, char	**raw_argv)
// {
// 	int	count;
// 	int	i;
// 	int	j;

// 	if (!argv || !*argv || !raw_argv)
// 		return (0);
// 	count = 0;
// 	i = 0;
// 	j = 0;
// 	while (argv[i] && raw_argv[j])
// 	{
// 		if (!*(argv[i]) && *(raw_argv[j]))
// 			count--;
// 		else if (ft_strcmp(argv[i], raw_argv[j]))
// 		{

// 		}
// 		else
// 			count++;
// 		i++;
// 		j++;
// 	}
// 	return (count);
// }

// int	get_split_index(char *expanded, char *raw)
// {
// 	char	quote;
// 	int		split_index;
// 	int		i;
// 	int		j;

// 	quote = 0;
// 	split_index = -1;
// 	i = 0;
// 	j = 0;
// 	while (expanded[i] && raw[j])
// 	{
// 		if (expanded[i] != raw[j])
// 		{
// 			while (!quote && (expanded[i] == ' ' || ))
// 		}
// 		while (expanded[i] && raw[j] && expanded[i] == raw[j])
// 		{
// 			update_quote_status(expanded[i], &quote);
// 			i++;
// 			j++;
// 		}
// 	}
// 	return (split_index);
// }

// int	resize_argv(t_data *data, char ***argv, char **raw_argv)
// {
// 	//get new argc
// 	//allocate new argv
// 	//if argv[i] == raw_argv[i] (minus quotes), simple strdup
// 	//else if argv[i] is empty remove it
// 	//else if argv[i] has IFS (space, \n, \t) and was not within quotes, split
// }
