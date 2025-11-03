/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:57:13 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/03 20:46:56 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_quotes(t_data *data, char **arg, t_arg_info *info)
{
	int		old_len;
	int		remove;
	char	*tmp;

	if (!arg || !*arg)
		return (0);
	remove = count_quotes(*arg);
	old_len = ft_strlen(*arg);
	if (old_len - remove <= 0)
		info->quote_map = ft_calloc(1, sizeof(int));
	else
		info->quote_map = ft_calloc(old_len - remove, sizeof(int));
	validate_malloc(data, info->quote_map, NULL);
	info->total_len = old_len - remove;
	if (!remove)
		return (0);
	tmp = ft_calloc(old_len - remove + 1, sizeof(char));
	validate_malloc(data, tmp, NULL);
	copy_without_quotes(tmp, *arg, info->quote_map);
	free(*arg);
	*arg = tmp;
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
		if (!quote && is_quote(arg[i]))
		{
			quote = arg[i];
			count++;
		}
		else if (quote && arg[i] == quote)
		{
			quote = 0;
			count++;
		}
		i++;
	}
	return (count);
}

void	copy_without_quotes(char *dest, char *src, int *quote_map)
{
	char	quote;
	int		i;
	int		j;

	quote = 0;
	i = 0;
	j = 0;
	while (src[i])
	{
		if (!quote && is_quote(src[i]))
			quote = src[i];
		else if (quote && src[i] == quote)
			quote = 0;
		else
		{
			dest[j] = src[i];
			quote_map[j] = is_quote(quote);
			j++;
		}
		i++;
	}
	dest[j] = '\0';
}

// PREVIOUS VERSION
// void	remove_quotes(t_data *data, char **arg, char *raw_arg)
// {
// 	int		i;
// 	int		j;
// 	int		*to_remove;
// 	char	quote;

// 	if (!arg || !*arg || !count_quotes(*arg))
// 		return ;
// 	to_remove = ft_calloc(ft_strlen(*arg), sizeof(int));
// 	i = 0;
// 	j = 0;
// 	quote = 0;
// 	while ((*arg)[i] && raw_arg[j])
// 	{
// 		if ((*arg)[i] != raw_arg[j])
// 			skip_expanded_segment(data, raw_arg + j, &i, &j);
// 		while ((*arg)[i] && raw_arg[j] && (*arg)[i] == raw_arg[j])
// 		{
// 			if (!quote && is_quote((*arg)[i]))
// 			{
// 				quote = (*arg)[i];
// 				to_remove[i] = 1;
// 			}
// 			else if (quote && (*arg)[i] == quote)
// 			{
// 				quote = 0;
// 				to_remove[i] = 1;
// 			}
// 			i++;
// 			j++;
// 		}
// 	}
// 	*arg = copy_without_quotes(data, *arg, to_remove);
// 	free(to_remove);
// }

// void	skip_expanded_segment(t_data *data, char *raw, int *i, int *j)
// {
// 	char	*key;
// 	char	*value;

// 	if (*raw == '$')
// 	{
// 		key = get_env_key(raw);
// 		validate_malloc(data, key, NULL);
// 		value = get_env_value(data->env_list, key + 1);
// 	}
// 	else
// 	{
// 		key = get_tilde_key(raw);
// 		validate_malloc(data, key, NULL);
// 		value = get_tilde_value(data, key);
// 	}
// 	*i = *i + ft_strlen(value);
// 	*j = *j + ft_strlen(key);
// 	free(key);
// }

// char	*copy_without_quotes(t_data *data, char *src, int *to_remove)
// {
// 	char	*dest;
// 	int		i;
// 	int		j;

// 	dest = malloc(sizeof(char) * (ft_strlen(src) - count_quotes(src) + 1));
// 	validate_malloc(data, dest, NULL);
// 	i = 0;
// 	j = 0;
// 	while (src[i])
// 	{
// 		if (!to_remove[i])
// 		{
// 			dest[j] = src[i];
// 			j++;
// 		}
// 		i++;

// 	}
// 	dest[j] = '\0';
// 	free(src);
// 	return (dest);
// }

// PREVIOUS VERSION
// char	remove_quotes(t_data *data, char **arg)
// {
// 	char	*tmp;
// 	char	quote;
// 	int		count;

// 	if (!arg || !*arg)
// 		return ;
// 	quote = 0;
// 	count = count_quotes(*arg, &quote);
// 	if (!count)
// 		return ;
// 	tmp = malloc(sizeof(char) * (ft_strlen(*arg) - count + 1));
// 	validate_malloc(data, tmp, NULL);
// 	copy_without_quotes(tmp, *arg, quote);
// 	free(*arg);
// 	*arg = tmp;
// }

// void	copy_without_quotes(char *dest, char *src, char quote)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (src[i])
// 	{
// 		if (src[i] != quote)
// 		{
// 			dest[j] = src[i];
// 			j++;
// 		}
// 		i++;
// 	}
// 	dest[j] = '\0';
// }
