/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:01:48 by beatde-a          #+#    #+#             */
/*   Updated: 2025/04/30 18:01:48 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	print_ptr(void *ptr)
{
	char	*buffer;
	int		len;
	int		res;

	if (!ptr)
		return (print_str("(nil)"));
	len = 0;
	res = print_str("0x");
	if (update_len(res, &len) < 0)
		return (-1);
	buffer = ft_itoa_base((uintptr_t)ptr, "0123456789abcdef");
	if (!buffer)
		return (-1);
	res = print_str(buffer);
	free (buffer);
	return (update_len(res, &len));
}

int	count_digits(uintptr_t n, size_t base_size)
{
	int	digits;

	digits = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		digits++;
		n /= base_size;
	}
	return (digits);
}

char	*ft_itoa_base(uintptr_t n, char *base)
{
	char	*buffer;
	size_t	base_size;
	int		digits;

	base_size = ft_strlen(base);
	digits = count_digits(n, base_size);
	buffer = malloc(digits + 1);
	if (!buffer)
		return (NULL);
	buffer[digits] = '\0';
	while (digits > 0)
	{
		digits--;
		buffer[digits] = base[n % base_size];
		n /= base_size;
	}
	return (buffer);
}
