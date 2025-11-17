/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:42:44 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/08 11:46:20 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_prefix(char *msg, char *label)
{
	if (!label || ft_strlen(label) > MAX_ARG_STRLEN)
		return ;
	ft_strlcat(msg, label, ERR_BUFFER_SIZE);
	ft_strlcat(msg, ": ", ERR_BUFFER_SIZE);
}

void	append_postfix(char *msg, char *label)
{
	if (!label || ft_strlen(label) > MAX_ARG_STRLEN)
		return ;
	ft_strlcat(msg, " ", ERR_BUFFER_SIZE);
	if (!is_quote(label[0]) || !label[1])
		ft_strlcat(msg, "\'", ERR_BUFFER_SIZE);
	ft_strlcat(msg, label, ERR_BUFFER_SIZE);
	if (!is_quote(label[0]) || !label[1])
		ft_strlcat(msg, "'", ERR_BUFFER_SIZE);
}

void	validate_malloc_env(t_data *data, void *ptr, t_env *node)
{
	if (ptr)
		return ;
	system_error(strerror(errno), "malloc");
	free_env_node(&node);
	error_exit(data, NULL);
}
