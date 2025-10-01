/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:34:32 by pedde-so          #+#    #+#             */
/*   Updated: 2025/09/22 10:23:29 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*correct_path(t_data *data, char *cmd)
{
	char	*full_path;
	int		i;
	char	**paths;
	int		access_res;

	if (!cmd)
		return (NULL);
	paths = ft_split(get_env_value(data->env_list, "PATH"), ':');
	i = 0;
	full_path = ft_strchr(cmd, '/');
	if (full_path)
		return (run_curr_dir(cmd));
	cmd = ft_strjoin("/", cmd);
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i++], cmd);
		if (full_path)
		{
			access_res = access(full_path, F_OK | X_OK);
			if (!access_res)
				return (free(cmd), (full_path));
			else
				free(full_path);
		}
	}
	ft_printf("%s: command not found\n", cmd + 1);
	return (NULL);
}

char	*run_curr_dir(char *cmd)
{
	int		access_res;

	access_res = access(cmd, F_OK | X_OK);
	if (errno == EACCES)
	{
		ft_printf("%s: Permission denied\n", cmd);
		return (NULL);
	}
	else if (errno == ENOENT)
	{
		ft_printf("%s: No such file or directory\n", cmd);
		return (NULL);
	}
	return (cmd);
}
