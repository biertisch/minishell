/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_argv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 11:30:04 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/04 11:30:04 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	copy_after_star(char **new_argv, char **argv, int i, int j)
{
	i++;
	while (argv[i])
	{
		new_argv[j] = ft_strdup(argv[i]);
		if (!new_argv[j])
			return (-1);
		i++;
		j++;
	}
	new_argv[j] = NULL;
	return (0);
}

static int	copy_entries(char **new_argv, t_list *entries, int j)
{
	while (entries)
	{
		new_argv[j] = ft_strdup((char *)entries->content);
		if (!new_argv[j])
			return (-1);
		entries = entries->next;
		j++;
	}
	return (j);
}

static int	copy_before_star(char **new_argv, char **argv, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		new_argv[j] = ft_strdup(argv[j]);
		if (!new_argv[j])
			return (-1);
		j++;
	}
	return (j);
}

static int	count_expanded_argv(char **argv, t_list *head)
{
	int		old_count;
	int		add_count;

	old_count = 0;
	while (argv[old_count])
		old_count++;
	add_count = 0;
	while (head)
	{
		add_count++;
		head = head->next;
	}
	return (old_count - 1 + add_count);
}

char	**update_argv(t_data *data, char **argv, int i, t_list *entries)
{
	char	**new_argv;
	int		count;
	int		j;

	count = count_expanded_argv(argv, entries);
	new_argv = malloc(sizeof(char *) * (count + 1));
	validate_malloc_wildcard(data, new_argv, entries, NULL);
	j = copy_before_star(new_argv, argv, i);
	if (j < 0)
		validate_malloc_wildcard(data, NULL, entries, new_argv);
	j = copy_entries(new_argv, entries, j);
	if (j < 0)
		validate_malloc_wildcard(data, NULL, entries, new_argv);
	if (copy_after_star(new_argv, argv, i, j))
		validate_malloc_wildcard(data, NULL, entries, new_argv);
	free_string_array(&argv);
	ft_lstclear(&entries, free);
	return (new_argv);
}
