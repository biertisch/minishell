/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_dollar3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 22:11:49 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/21 22:38:40 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	expand_redir_variable(t_data *data, char **file, int i)
{
	char	*key;
	char	*value;

	key = get_env_key(*file + i + 1);
	validate_malloc(data, key, NULL);
	value = get_env_value(data->env_list, key);
	if ((!value && *file[0] != '"') || (value && ft_strchr(value, ' ') && *file[0] != '"'))
	{
		free(key);
		return (internal_error(data, ERR_2, NULL, NULL));
	}
	*file = update_arg(*file, i, key, value);
	validate_malloc(data, *file, key);
	free(key);
	return (0);
}

int	expand_dollar_redir(t_data *data, char **file)
{
	int	i;

	if (!file || !*file || (*file)[0] == '\'')
		return (0);
	i = 0;
	while (*file && (*file)[i])
	{
		if ((*file)[i] == '$' && (*file)[i + 1] && (*file)[i + 1] != ' '
			&& (*file)[i + 1] != '"')
		{
			if ((*file)[i + 1] == '?')
				expand_exit_status(data, file, i);
			else if (expand_redir_variable(data, file, i))
				return (-1);
			i = -1;
		}
		i++;
	}
	return (0);
}
