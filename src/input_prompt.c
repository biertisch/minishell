/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 19:24:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/30 17:58:41 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_prompt(t_data *data)
{
	char	shell[16];
	char	*pwd;

	if (data->prompt)
		free(data->prompt);
	ft_strlcpy(shell, "🌱 minishell:", 16);
	pwd = get_prompt_pwd(data);
	data->prompt = malloc(ft_strlen(shell) + ft_strlen(pwd) + 3);
	validate_malloc(data, data->prompt, NULL);
	build_prompt(data->prompt, shell, pwd);
}

char	*get_prompt_pwd(t_data *data)
{
	char	*pwd;
	char	*home;
	int		home_len;

	pwd = get_env_value_modified(data, "PWD");
	home = get_env_value(data->env_list, "HOME");
	home_len = ft_strlen(home);
	if (home && !ft_strncmp(pwd, home, home_len))
	{
		pwd[0] = '~';
		ft_memmove(pwd + 1, pwd + home_len, ft_strlen(pwd) - home_len);
		pwd[ft_strlen(pwd) - home_len + 1] = '\0';
	}
	return (pwd);
}

char	*get_env_value_modified(t_data *data, char *key)
{
	char	*value;

	value = get_env_value(data->env_list, key);
	if (!value)
		return ("unknown");
	return (value);
}

void	build_prompt(char *prompt, char *shell, char *pwd)
{
	ft_strlcpy(prompt, shell, ft_strlen(shell) + 1);
	ft_strlcat(prompt, pwd, ft_strlen(prompt) + ft_strlen(pwd) + 1);
	ft_strlcat(prompt, "$ ", ft_strlen(prompt) + 3);
}
