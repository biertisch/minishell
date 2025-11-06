/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_metadata.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 21:56:58 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/06 19:47:02 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_expand_metadata(t_data *data, t_metadata *info, char *arg)
{
	info->total_len = ft_strlen(arg);
	free(info->expand_map);
	if (info->total_len < 1)
		info->expand_map = ft_calloc(1, sizeof(int));
	else
		info->expand_map = ft_calloc(info->total_len, sizeof(int));
	validate_malloc(data, info->expand_map, NULL);
	free(info->key);
	info->key = NULL;
	free(info->value);
	info->value = NULL;
	return (0);
}

// int	rebuild_quote_map(t_data *data, t_metadata *info, int start)
// {
// 	int	*new_map;
// 	int	old_len;
// 	int	status;
// 	int	i;
// 	int	j;

// 	old_len = info->total_len + info->key_len - info->value_len;
// 	if (info->total_len == 0)
// 		return (handle_empty_quote_map(data, info, start, old_len));
// 	new_map = ft_calloc(info->total_len, sizeof(int));
// 	validate_malloc(data, new_map, NULL);
// 	i = 0;
// 	j = 0;
// 	while (i < start)
// 		new_map[i++] = info->quote_map[j++];
// 	status = get_quote_status(info->quote_map, j, old_len);
// 	while (i < start + info->value_len)
// 		new_map[i++] = status;
// 	j += info->key_len;
// 	while (i < info->total_len && j < old_len)
// 		new_map[i++] = info->quote_map[j++];
// 	free(info->quote_map);
// 	info->quote_map = new_map;
// 	return (0);
// }

// int	handle_empty_quote_map(t_data *data, t_metadata *info, int start,
// 	int old_len)
// {
// 	int	*new_map;

// 	new_map = ft_calloc(1, sizeof(int));
// 	validate_malloc(data, new_map, NULL);
// 	if (start < old_len)
// 		new_map[0] = info->quote_map[start];
// 	else
// 		new_map[0] = 0;
// 	free(info->quote_map);
// 	info->quote_map = new_map;
// 	return (0);
// }

int	rebuild_expand_map(t_data *data, t_metadata *info, int start, int type)
{
	int	*new_map;
	int	old_len;
	int	i;
	int	j;

	if (info->total_len == 0)
		return (handle_empty_expand_map(data, info));
	old_len = info->total_len + info->key_len - info->value_len;
	new_map = ft_calloc(info->total_len, sizeof(int));
	validate_malloc(data, new_map, NULL);
	i = 0;
	j = 0;
	while (info->expand_map && i < start && j < old_len)
		new_map[i++] = info->expand_map[j++];
	i = start;
	while (info->value_len && i < start + info->value_len)
		new_map[i++] = type;
	j += info->key_len;
	while (info->expand_map && i < info->total_len && j < old_len)
		new_map[i++] = info->expand_map[j++];
	free(info->expand_map);
	info->expand_map = new_map;
	return (0);
}

int	handle_empty_expand_map(t_data *data, t_metadata *info)
{
	int	*new_map;

	new_map = ft_calloc(1, sizeof(int));
	validate_malloc(data, new_map, NULL);
	new_map[0] = 1;
	free(info->expand_map);
	info->expand_map = new_map;
	return (0);
}
