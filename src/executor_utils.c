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

	access(cmd, F_OK | X_OK);
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

void	check_for_variables(t_data *data, t_stack **stack)
{
	int	i;
	int	j;
	t_env	*new_node;

	i = 0;
	while (is_new_var((*stack)->node->argv[i]))
		i++;
	if (!i)
		return ;
	j = 0;
	while (j < i)
	{
		new_node = create_env_node(NULL, NULL, 0);
		validate_malloc(data, new_node, NULL);
		split_env_entry(data, (*stack)->node->argv[j], new_node);
		j++;
	}
	(*stack)->node->argv = ft_shrink_split((*stack)->node->argv, j - 1, ft_splitlen((*stack)->node->argv) - 1);
}
