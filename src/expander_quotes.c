/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:57:13 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/02 16:40:10 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_quotes(t_data *data, char **arg, char *raw_arg)
{
	int		i;
	int		j;
	int		*to_remove;
	char	quote;

	if (!arg || !*arg || !count_quotes(*arg))
		return ;
	to_remove = ft_calloc(ft_strlen(*arg), sizeof(int));		
	i = 0;
	j = 0;
	quote = 0;
	while ((*arg)[i] && raw_arg[j])
	{
		if ((*arg)[i] != raw_arg[j])
			pass_expanded_segment(data, raw_arg + j, &i, &j);
		while ((*arg)[i] && raw_arg[j] && (*arg)[i] == raw_arg[j])
		{
			if (!quote && is_quote((*arg)[i]))
			{
				quote = (*arg)[i];
				to_remove[i] = 1;
			}
			else if (quote && (*arg)[i] == quote)
			{
				quote = 0;
				to_remove[i] = 1;
			}
			i++;
			j++;
		}
	}
	*arg = copy_without_quotes(data, *arg, to_remove);
	free(to_remove);
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

void	pass_expanded_segment(t_data *data, char *raw, int *i, int *j)
{
	char	*key;
	char	*value;

	if (*raw == '$')
	{
		key = get_env_key(raw);
		validate_malloc(data, key, NULL);
		value = get_env_value(data->env_list, key + 1);
	}
	else
	{
		key = get_tilde_key(raw);
		validate_malloc(data, key, NULL);
		value = get_tilde_value(data, key);
	}
	*i = *i + ft_strlen(value);
	*j = *j + ft_strlen(key);
	free(key);
}

char	*copy_without_quotes(t_data *data, char *src, int *to_remove)
{
	char	*dest;
	int		i;
	int		j;

	dest = malloc(sizeof(char) * (ft_strlen(src) - count_quotes(src) + 1));
	validate_malloc(data, dest, NULL);
	i = 0;
	j = 0;
	while (src[i])
	{
		if (!to_remove[i])
		{
			dest[j] = src[i];
			j++;			
		}
		i++;

	}
	dest[j] = '\0';
	free(src);
	return (dest);
}


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

// int	count_quotes(char *arg, char *quote)
// {
// 	int	count;
// 	int	i;

// 	count = 0;
// 	i = 0;
// 	while (arg[i])
// 	{
// 		if (!*quote && is_quote(arg[i]))
// 			*quote = arg[i];
// 		if (*quote && arg[i] == *quote)
// 			count++;
// 		i++;
// 	}
// 	return (count);
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
