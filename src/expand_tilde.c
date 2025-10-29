/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:57:45 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/21 18:11:09 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*replace_tilde_value(char *arg, int key_len, char *value)
{
	char	*new_arg;
	int		new_len;
	int		value_len;

	value_len = ft_strlen(value);
	new_len = ft_strlen(arg) - key_len + value_len;
	new_arg = malloc(sizeof(char) * (new_len + 1));
	if (!new_arg)
		return (NULL);
	ft_strlcpy(new_arg, value, value_len + 1);
	ft_strlcat(new_arg, arg + key_len, new_len + 1);
	free(arg);
	return (new_arg);
}

static char	*get_value(t_data *data, char *arg, int *key_len)
{
	if (!ft_strcmp(arg, "~+") || !ft_strncmp(arg, "~+/", 3))
	{
		*key_len = 2;
		return (get_env_value(data->env_list, "PWD"));
	}
	else if (!ft_strcmp(arg, "~-") || !ft_strncmp(arg, "~-/", 3))
	{
		*key_len = 2;
		return (get_env_value(data->env_list, "OLDPWD"));
	}
	else if (!ft_strcmp(arg, "~") || !ft_strncmp(arg, "~/", 2))
	{
		*key_len = 1;
		return (get_env_value(data->env_list, "HOME"));
	}
	return (NULL);
}

void	expand_tilde(t_data *data, char **arg)
{
	char	*value;
	int		key_len;

	if (!arg || !*arg || **arg != '~')
		return ;
	value = get_value(data, *arg, &key_len);
	if (value)
	{
		*arg = replace_tilde_value(*arg, key_len, value);
		validate_malloc(data, *arg, NULL);
	}
}
