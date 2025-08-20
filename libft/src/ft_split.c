/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:59:56 by beatde-a          #+#    #+#             */
/*   Updated: 2025/04/16 14:12:48 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static void	clear(char **arr, size_t i)
{
	while (i > 0)
	{
		free(arr[i - 1]);
		i--;
	}
	free(arr);
	arr = NULL;
}

static int	alt_strlen(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	**build_arr(char const *s, char c, size_t count)
{
	char	**arr;
	size_t	i;
	size_t	j;

	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count && s[j])
	{
		if (s[j] != c && (j == 0 || s[j - 1] == c))
		{
			arr[i] = ft_substr(s, j, alt_strlen(s + j, c));
			if (!arr[i])
			{
				clear(arr, i);
				return (NULL);
			}
			i++;
		}
		j++;
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	count;
	size_t	i;

	if (!s)
		return (NULL);
	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	res = build_arr(s, c, count);
	return (res);
}
