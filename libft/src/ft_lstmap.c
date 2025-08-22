/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 16:18:09 by beatde-a          #+#    #+#             */
/*   Updated: 2025/06/03 19:22:19 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static void	*set_content(t_list *lst, void *(*f)(void *))
{
	void	*new_content;

	if (f)
		new_content = f(lst->content);
	else
		new_content = ft_strdup(lst->content);
	if (!new_content)
		return (NULL);
	return (new_content);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*next;
	void	*new_content;

	if (!lst || !del)
		return (NULL);
	new_content = set_content(lst, f);
	if (!new_content)
		return (NULL);
	new = ft_lstnew(new_content);
	if (!new)
	{
		del(new_content);
		return (NULL);
	}
	next = ft_lstmap(lst->next, f, del);
	if (lst->next && !next)
	{
		ft_lstclear(&new, del);
		return (NULL);
	}
	new->next = next;
	return (new);
}
