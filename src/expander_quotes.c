/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:57:13 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/06 18:26:20 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	copy_without_quotes(char *dest, char *src, char quote)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (src[i] != quote)
		{
			dest[j] = src[i];
			j++;
		}
		i++;
	}
	dest[j] = '\0';
}

static int	count_quotes(char *arg, char *quote)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (arg[i])
	{
		if (!*quote && is_quote(arg[i]))
			*quote = arg[i];
		if (*quote && arg[i] == *quote)
			count++;
		i++;
	}
	return (count);
}

void	remove_quotes(t_data *data, char **arg)
{
	char	*tmp;
	char	quote;
	int		count;

	quote = 0;
	count = count_quotes(*arg, &quote);
	if (!count)
		return ;
	tmp = malloc(sizeof(char) * (ft_strlen(*arg) - count + 1));
	validate_malloc(data, tmp, NULL);
	copy_without_quotes(tmp, *arg, quote);
	free(*arg);
	*arg = tmp;
}
