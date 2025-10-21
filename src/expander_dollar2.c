/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:55:42 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/21 22:09:13 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	copy_to_array(char **dest, char **src, int size)
{
	int	i;

	if (size == 0 || !dest || !src)
		return (0);
	i = 0;
	while (i < size && src[i])
	{
		dest[i] = ft_strdup(src[i]);
		if (!dest[i])
		{
			i--;
			while (i >= 0)
			{
				free(dest[i]);
				i--;
			}
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	get_argv_count(char **old_argv, char **add_argv, int *add_count)
{
	int	old_count;

	old_count = 0;
	while (old_argv && old_argv[old_count])
		old_count++;
	*add_count = 0;
	while (add_argv && add_argv[*add_count])
		(*add_count)++;
	return (old_count + *add_count - 1);
}

static int	build_argv(char **new_argv, char **old_argv, char **add_argv, int i)
{
	int		add_count;
	int		count;

	count = get_argv_count(old_argv, add_argv, &add_count);
	if (copy_to_array(new_argv, old_argv, i))
		return (-1);
	if (copy_to_array(new_argv + i, add_argv, add_count))
		return (-1);
	if (i + add_count < count)
		if (copy_to_array(new_argv + i + add_count, old_argv + i + 1, count))
			return (-1);
	return (0);
}

static char **get_add_argv(char *value)
{
	char	**add_argv;

	if (value)
		add_argv = ft_split(value, ' ');
	else
		add_argv = NULL;
	return (add_argv);
}

char	**update_argv_dollar(char **old_argv, int i, char *value)
{
	char	**new_argv;
	char	**add_argv;
	int		add_count;
	int		count;

	add_argv = get_add_argv(value);
	count = get_argv_count(old_argv, add_argv, &add_count);
	new_argv = malloc(sizeof(char *) * (count + 1));
	if (!new_argv)
	{
		free_string_array(&add_argv);
		return (NULL);
	}
	new_argv[count] = NULL;
	if (build_argv(new_argv, old_argv, add_argv, i))
	{
		free(new_argv);
		free_string_array(&add_argv);
		return (NULL);
	}
	free_string_array(&old_argv);
	free_string_array(&add_argv);
	return (new_argv);
}
