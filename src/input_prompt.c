/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 19:24:02 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/06 14:44:36 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_prompt(t_data *data)
{
	char	shell[16];
	char	*pwd;
	int		shell_len;
	int		pwd_len;

	free(data->prompt);
	ft_strlcpy(shell, "🌱 minishell:", 16);
	shell_len = ft_strlen(shell);
	pwd = get_prompt_pwd(data);
	validate_malloc(data, pwd, NULL);
	pwd_len = ft_strlen(pwd);
	data->prompt = malloc(shell_len + pwd_len + 3);
	validate_malloc(data, data->prompt, pwd);
	ft_strlcpy(data->prompt, shell, shell_len + 1);
	ft_strlcat(data->prompt, pwd, shell_len + pwd_len + 1);
	ft_strlcat(data->prompt, "$ ", shell_len + pwd_len + 3);
	free(pwd);
}

char	*get_prompt_pwd(t_data *data)
{
	char	*display;
	char	*pwd;
	char	*home;
	int		pwd_len;
	int		home_len;

	pwd = get_env_value(data->env_list, "PWD");
	if (!pwd)
		return (ft_strdup("unknown"));
	home = get_env_value(data->env_list, "HOME");
	home_len = ft_strlen(home);
	if (!home && ft_strncmp(pwd, home, home_len))
		return (ft_strdup(pwd));
	pwd_len = ft_strlen(pwd);
	display = ft_calloc(pwd_len - home_len + 2, sizeof(char));
	if (!display)
		return (NULL);
	display[0] = '~';
	ft_strlcpy(display + 1, pwd + home_len, pwd_len - home_len + 1);
	return (display);
}
