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

char	*correct_path(t_data *data, t_stack **stack, char *cmd)
{
	char	*full_path;
	int		i;
	char	**paths;
	char	*slash_path;
	int		access_res;

	if (!cmd)
		return (NULL);
	i = 0;
	full_path = ft_strchr(cmd, '/');
	if (full_path)
		return (run_curr_dir(data, stack, cmd));
	paths = ft_split(get_env_value(data->env_list, "PATH"), ':');
	slash_path = ft_strjoin("/", cmd);
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i++], slash_path);
		if (full_path)
		{
			access_res = access(full_path, F_OK | X_OK);
			if (!access_res)
				return (free(slash_path), (full_path));
			else if (errno == ENOENT)
				free(full_path);
			else
			{
				ft_splitfree(paths);
				free(slash_path);
				executor_child_errno(data, stack, cmd);
			}
		}
	}
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	write(STDERR_FILENO, ": command not found\n", 20);
	ft_splitfree(paths);
	free(slash_path);
	free_all(data);
	free_stack(stack);
	exit(127);
	return (NULL);
}

char	*run_curr_dir(t_data *data, t_stack **stack, char *cmd)
{
	int	access_res;

	access_res = access(cmd, F_OK | X_OK);
	if (access_res == -1)
		executor_child_errno(data, stack, cmd);
	return (cmd);
}

void	executor_child_errno(t_data *data, t_stack **stack, char *cmd)
{	
	write(STDERR_FILENO, cmd, ft_strlen(cmd));
	free_all(data);
	free_stack(stack);
	if (errno == EACCES)
		write(STDERR_FILENO, ": Permission denied\n", 20);
	else if (errno == ENOENT)
	{
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		exit(127);
	}
	else if (errno == ENOTDIR)
		write(STDERR_FILENO, ": Not a directory\n", 18);
	else if (errno == ELOOP)
		write(STDERR_FILENO, ": Too many levels of symbolic links\n", 36);
	else if (errno == ENAMETOOLONG)
		write(STDERR_FILENO, ": File name too long\n", 21);
	exit(126);
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
