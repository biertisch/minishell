/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:21:24 by beatde-a          #+#    #+#             */
/*   Updated: 2025/04/16 12:21:51 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static size_t	count_digits(long int n)
{
	size_t	count;

	count = 0;
	if (n <= 0)
		count++;
	while (n != 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char		*res;
	long int	nbr;
	size_t		count;

	nbr = n;
	count = count_digits(nbr);
	res = malloc(count + 1);
	if (!res)
		return (NULL);
	res[count] = '\0';
	if (nbr < 0)
	{
		res[0] = '-';
		nbr *= -1;
	}
	while (nbr > 9)
	{
		count--;
		res[count] = nbr % 10 + '0';
		nbr /= 10;
	}
	res[--count] = nbr + '0';
	return (res);
}
