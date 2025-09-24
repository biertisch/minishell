/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:41:44 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/24 18:41:46 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*result;
	int			bytes_read;

	result = NULL;
	bytes_read = 1;
	if (!buffer[0])
		bytes_read = -2;
	if (fd < 0 || fd >= FOPEN_MAX || BUFFER_SIZE < 1)
		return (NULL);
	return (get_next_line_cont(fd, buffer, result, bytes_read));
}
