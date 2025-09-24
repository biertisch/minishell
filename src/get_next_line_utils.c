/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:43:04 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/24 18:43:05 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_find_init_nl(char *buff)
{
	int	i;

	i = 0;
	while (*(buff + i) && *(buff + i) != '\n')
		i++;
	return (i);
}

char	*ft_handle_new_line(char *buff, char *result, int i)
{
	int	buffer_index;
	int	result_index;

	result = ft_gnl_realloc(result, i, &result_index);
	if (!result)
		return (NULL);
	buffer_index = 0;
	while (buffer_index <= i)
	{
		*(result + result_index) = *(buff + buffer_index);
		*(buff + buffer_index) = '\0';
		buffer_index++;
		result_index++;
	}
	*(result + result_index) = '\0';
	return (result);
}

char	*ft_process_buffer(char *buff, int i)
{
	int	start;

	start = 0;
	while (*(buff + i) && i < BUFFER_SIZE)
	{
		*(buff + start) = *(buff + i);
		*(buff + i) = '\0';
		i++;
		start++;
	}
	return (buff);
}

char	*get_next_line_cont(int fd, char *buffer, char *result, int bytes_read)
{
	int	i;

	result = NULL;
	while (bytes_read)
	{
		if (bytes_read != -2)
		{
			i = ft_find_init_nl(buffer);
			result = ft_handle_new_line(buffer, result, i);
			if (!result)
				return (NULL);
			buffer = ft_process_buffer(buffer, i + 1);
			if (*(result + i) && i < BUFFER_SIZE)
				return (result);
		}
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(result), NULL);
		buffer[bytes_read] = '\0';
	}
	if (!bytes_read && !result)
		return (NULL);
	return (result);
}

char	*ft_gnl_realloc(char *result, int i, int *r)
{
	char	*new_result;

	*r = 0;
	if (result)
		while (*(result + *r))
			(*r)++;
	new_result = malloc(i + *r + 2);
	if (!new_result)
	{
		if (result)
			free(result);
		return (NULL);
	}
	if (!result)
		return (new_result);
	*r = 0;
	while (*(result + *r))
	{
		*(new_result + *r) = *(result + *r);
		(*r)++;
	}
	free(result);
	return (new_result);
}
