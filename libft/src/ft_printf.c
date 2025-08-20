/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:47:26 by beatde-a          #+#    #+#             */
/*   Updated: 2025/04/30 12:17:19 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	update_len(int res, int *len)
{
	if (res < 0)
		return (-1);
	*len += res;
	return (*len);
}

int	convert(va_list args, char type)
{
	if (type == '%')
		return (print_char('%'));
	if (type == 'c')
		return (print_char(va_arg(args, int)));
	if (type == 's')
		return (print_str(va_arg(args, char *)));
	if (type == 'd' || type == 'i')
		return (print_int(va_arg(args, int)));
	if (type == 'u')
		return (print_uint(va_arg(args, unsigned int)));
	if (type == 'x' || type == 'X')
		return (print_hex(va_arg(args, unsigned int), type));
	if (type == 'p')
		return (print_ptr(va_arg(args, void *)));
	return (ft_printf("%%%c", type));
}

int	lone_percent(const char *format)
{
	int	i;

	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format [i - 1] != '%'
			&& !format[i + 1])
			return (1);
		i++;
	}
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		len;
	int		res;
	int		i;

	if (!format)
		return (-1);
	len = 0;
	i = 0;
	va_start(args, format);
	if (lone_percent(format))
		return (-1);
	while (format[i])
	{
		if (format[i] == '%')
			res = convert(args, format[++i]);
		else
			res = print_char(format[i]);
		if (update_len(res, &len) < 0)
			return (-1);
		i++;
	}
	va_end(args);
	return (len);
}
