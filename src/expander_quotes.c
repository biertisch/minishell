/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:57:13 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 16:43:59 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_quote_map(t_data *data, char *arg, t_metadata *info)
{
	int		i;
	char	quote;

	free(info->quote_map);
	if (!info->total_len)
		info->quote_map = ft_calloc(1, sizeof(int));
	else
		info->quote_map = ft_calloc(info->total_len, sizeof(int));
	validate_malloc(data, info->quote_map, NULL);
	quote = 0;
	i = 0;
	while (arg[i])
	{
		toggle_quote(arg[i], &quote);
		info->quote_map[i] = is_quote(quote);
		i++;
	}
	return (0);
}

int	remove_quotes(t_data *data, char **arg, t_metadata *info)
{
	char	*new_arg;
	int		to_remove;

	if (!arg || !*arg || !info)
		return (0);
	to_remove = count_quotes(*arg);
	info->total_len = ft_strlen(*arg) - to_remove;
	free(info->quote_map);
	if (info->total_len <= 0)
		info->quote_map = ft_calloc(1, sizeof(int));
	else
		info->quote_map = ft_calloc(info->total_len, sizeof(int));
	validate_malloc(data, info->quote_map, NULL);
	if (!to_remove)
		return (0);
	new_arg = ft_calloc(info->total_len + 1, sizeof(char));
	validate_malloc(data, new_arg, NULL);
	copy_without_quotes(new_arg, *arg, info->quote_map);
	free(*arg);
	*arg = new_arg;
	return (0);
}

int	count_quotes(char *arg)
{
	char	quote;
	int		count;
	int		i;

	quote = 0;
	count = 0;
	i = 0;
	while (arg[i])
	{
		if (toggle_quote(arg[i], &quote))
			count++;
		i++;
	}
	return (count);
}

int	copy_without_quotes(char *dest, char *src, int *map)
{
	char	quote;
	int		i;
	int		j;

	quote = 0;
	i = 0;
	j = 0;
	while (src[i])
	{
		if (!toggle_quote(src[i], &quote))
		{
			dest[j] = src[i];
			map[j] = is_quote(quote);
			j++;
		}
		i++;
	}
	dest[j] = '\0';
	return (0);
}
