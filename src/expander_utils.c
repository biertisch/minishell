/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:26:29 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/02 14:55:39 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_string_array(char **src)
{
	char	**dest;
	int		count;
	int		i;

	if (!src)
		return (NULL);
	count = get_argc(src);
	dest = malloc(sizeof(char *) * (count + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = ft_strdup(src[i]);
		if (!dest[i])
		{
			free_unfinished_string_array(dest, i);
			return (NULL);
		}
		i++;
	}
	dest[i] = NULL;
	return (dest);
}

void	free_unfinished_string_array(char **arr, int size)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (i < size)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
