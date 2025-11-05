/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 22:02:28 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/04 22:03:33 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arg_disappears(char *arg, t_metadata *info)
{
	return (!*arg && info->expand_map && info->expand_map[0] == 1
		&& info->quote_map && !info->quote_map[0]);
}

int	is_ifs(char *arg, t_metadata *info, int i)
{
	return (arg[i] && (arg[i] == ' ' || arg[i] == '\t' || arg[i] == '\n')
		&& info->expand_map && info->expand_map[i] == 1
		&& info->quote_map && !info->quote_map[i]);
}

int	has_ifs(char *arg, t_metadata *info, int i)
{
	if (!arg)
		return (-1);
	while (arg[i])
	{
		if (is_ifs(arg, info, i))
			return (i);
		i++;
	}
	return (-1);
}

int	count_fields(char *arg, t_metadata *info)
{
	int	i;
	int	count;
	int	in_field;

	if (!arg)
		return (0);
	count = 0;
	in_field = 0;
	i = 0;
	while (arg[i])
	{
		if (!is_ifs(arg, info, i) && !in_field)
		{
			count++;
			in_field = 1;
		}
		else if (is_ifs(arg, info, i))
			in_field = 0;
		i++;
	}
	if (count == 0 && !arg_disappears(arg, info))
		return (1);
	return (count);
}

int	split_on_ifs(char **dest, char *src, t_metadata *src_info,
	t_metadata *dest_info)
{
	int	start;
	int	end;
	int	i;

	start = 0;
	i = 0;
	while (src[start])
	{
		while (is_ifs(src, src_info, start))
			start++;
		if (!src[start])
			break ;
		end = has_ifs(src, src_info, start);
		if (end == -1)
			end = ft_strlen(src);
		dest[i] = ft_substr(src, start, end);
		if (!dest[i] || copy_arg_info(dest_info + i, src_info, start, end))
			return (-1);
		i++;
		start = end;
	}
	dest[i] = NULL;
	return (i);
}
