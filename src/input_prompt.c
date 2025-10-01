/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 19:24:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/09/30 19:55:08 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	build_prompt(char *prompt, char *user, char *hostname, char *pwd)
{
	ft_strlcpy(prompt, user, ft_strlen(user) + 1);
	ft_strlcat(prompt, "@", ft_strlen(prompt) + 2);
	ft_strlcat(prompt, hostname, ft_strlen(prompt) + ft_strlen(hostname) + 1);
	ft_strlcat(prompt, ":", ft_strlen(prompt) + 2);
	ft_strlcat(prompt, pwd, ft_strlen(prompt) + ft_strlen(pwd) + 1);
	ft_strlcat(prompt, "$ ", ft_strlen(prompt) + 3);
}

static char	*get_prompt_pwd(t_data *data)
{
	char	*pwd;
	char	*home;
	int		home_len;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		system_error(data, "getcwd");
		error_exit(data);
	}
	home = get_env_value(data->env_list, "HOME");
	home_len = ft_strlen(home);
	if (home && !ft_strncmp(pwd, home, home_len))
	{
		pwd[0] = '~';
		ft_memmove(pwd + 1, pwd + home_len, ft_strlen(pwd) - home_len);
	}
	return (pwd);
}

static char	*get_value(t_data *data, char *key)
{
	char	*value;

	value = get_env_value(data->env_list, key);
	if (!value)
		return ("unknown");
	return (value);
}

void	update_prompt(t_data *data)
{
	char	*user;
	char	*hostname;
	char	*pwd;

	if (data->prompt)
		free(data->prompt);
	user = get_value(data, "USER");
	hostname = get_value(data, "HOSTNAME");
	pwd = get_prompt_pwd(data);
	data->prompt = malloc(ft_strlen(user) + ft_strlen(hostname)
			+ ft_strlen(pwd) + 5);
	validate_malloc(data, data->prompt, NULL);
	build_prompt(data->prompt, user, hostname, pwd);
	free(pwd);
}
