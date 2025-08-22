/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:57:15 by beatde-a          #+#    #+#             */
/*   Updated: 2025/04/30 11:46:13 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	print_char(char c)
{
	return (write(1, &c, 1));
}

int	print_str(char *s)
{
	if (!s)
		return (write(1, "(null)", 6));
	return (write(1, s, ft_strlen(s)));
}

int	print_int(int n)
{
	char		*buffer;
	long int	nbr;
	int			len;
	int			res;

	nbr = n;
	len = 0;
	if (nbr < 0)
	{
		res = print_char('-');
		if (update_len(res, &len) < 0)
			return (-1);
		nbr *= -1;
	}
	buffer = ft_itoa_base((uintptr_t)nbr, "0123456789");
	if (!buffer)
		return (-1);
	res = print_str(buffer);
	free (buffer);
	return (update_len(res, &len));
}

int	print_uint(unsigned int n)
{
	char	*buffer;
	int		len;

	buffer = ft_itoa_base((uintptr_t)n, "0123456789");
	if (!buffer)
		return (-1);
	len = print_str(buffer);
	free (buffer);
	return (len);
}

int	print_hex(unsigned int n, char type)
{
	char	*buffer;
	int		len;

	if (type == 'x')
		buffer = ft_itoa_base((uintptr_t)n, "0123456789abcdef");
	else
		buffer = ft_itoa_base((uintptr_t)n, "0123456789ABCDEF");
	if (!buffer)
		return (-1);
	len = print_str(buffer);
	free (buffer);
	return (len);
}
