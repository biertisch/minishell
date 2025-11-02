/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 13:34:32 by pedde-so          #+#    #+#             */
/*   Updated: 2025/10/29 22:11:43 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*correct_path(t_data *data, t_stack **stack, char *cmd)
{
	char	*full_path;
	int		i;
	char	**paths;
	char	*slash_path;

	if (!cmd)
		return (NULL);
	i = 0;
	full_path = ft_strchr(cmd, '/');
	if (full_path)
		return (run_curr_dir(data, stack, cmd));
	slash_path = ft_strjoin("/", cmd);
	if (!ft_strcmp(slash_path, "/"))
		cmd_not_found(data, stack, NULL, slash_path);
	validate_malloc_execute(data, stack, slash_path, cmd);
	if (get_env_value(data->env_list, "PATH"))
		paths = ft_split(get_env_value(data->env_list, "PATH"), ':');
	else
		paths = ft_split("/usr/local/bin:/usr/bin:/bin", ':');
	if (!paths)
	{
		free(slash_path);
		validate_malloc_execute(data, stack, paths, cmd);
	}
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i++], slash_path);
		if (full_path)
		{
			if (!access(full_path, F_OK | X_OK))
			{
				ft_splitfree(paths);
				return (free(slash_path), (full_path));
			}
			else if (errno == ENOENT)
				free(full_path);
			else
			{
				ft_splitfree(paths);
				free(slash_path);
				executor_child_errno(data, stack, cmd);
			}
		}
		else
		{
			ft_splitfree(paths);
			free(cmd);
			validate_malloc_execute(data, stack, full_path, slash_path);
		}
	}
	cmd_not_found(data, stack, paths, slash_path);
	return (NULL);
}

void	cmd_not_found(t_data *data, t_stack **stack, char **paths, char *slash_path)
{
	write(STDERR_FILENO, (*stack)->node->argv[get_first_command(data, stack)], ft_strlen((*stack)->node->argv[get_first_command(data, stack)]));
	write(STDERR_FILENO, ": command not found\n", 20);
	if ((*stack)->node->redir)
	{
		if ((*stack)->node->redir->type == REDIR_OUT)
			(*stack)->node->redir->fd = open((*stack)->node->redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if ((*stack)->node->redir->type == APPEND)
			(*stack)->node->redir->fd = open((*stack)->node->redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		close((*stack)->node->redir->fd);
	}
	close_all_open_redir_ends(data);
	close_all_pipe_ends(stack);
	ft_splitfree(paths);
	free(slash_path);
	free_stack(stack);
	free_all(data);
	exit(127);

}

char	*run_curr_dir(t_data *data, t_stack **stack, char *cmd)
{
	char	*cmd_res;

	if (access(cmd, F_OK | X_OK) == -1)
		executor_child_errno(data, stack, cmd);
	cmd_res = ft_strdup(cmd);
	validate_malloc_execute(data, stack, cmd_res, NULL);
	return (cmd_res);
}

void	executor_child_errno(t_data *data, t_stack **stack, char *cmd)
{
	(void)cmd;
	write(STDERR_FILENO, (*stack)->node->argv[0], ft_strlen((*stack)->node->argv[0]));
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

void	executor_cleanup(t_data *data, t_stack **stack, char *cmd)
{
	close_all_open_redir_ends(data);
	close_all_pipe_ends(stack);
	if (cmd)
		free(cmd);
	free_stack(stack);
	free_all(data);
}
