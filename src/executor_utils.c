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

	if (ft_strchr(cmd, '/'))
		return (run_curr_dir(data, stack, cmd));
	slash_path = get_slash_path(data, stack, cmd);
	paths = get_path_split(data, stack, slash_path);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i++], slash_path);
		if (!full_path || !access(full_path, F_OK | X_OK))
		{
			ft_splitfree(paths);
			validate_malloc_execute(data, stack, full_path, slash_path);
			return (free(slash_path), (full_path));
		}
		else if (errno == ENOENT)
			free(full_path);
		else
			executor_child_errno(data, stack, slash_path, paths);
	}
	return (cmd_not_found(data, stack, paths, slash_path));
}

char	*get_slash_path(t_data *data, t_stack **stack, char *cmd)
{
	char	*slash_path;

	if (!cmd)
		return (NULL);
	slash_path = ft_strjoin("/", cmd);
	validate_malloc_execute(data, stack, slash_path, cmd);
	if (!ft_strcmp(slash_path, "/") || !ft_strcmp(slash_path, "/.."))
		cmd_not_found(data, stack, NULL, slash_path);
	return (slash_path);
}

char	**get_path_split(t_data *data, t_stack **stack, char *slash_path)
{
	char	**paths;

	if (get_env_value(data->env_list, "PATH"))
		paths = ft_split(get_env_value(data->env_list, "PATH"), ':');
	else
		paths = ft_split("/usr/local/bin:/usr/bin:/bin", ':');
	if (!paths)
		validate_malloc_execute(data, stack, paths, slash_path);
	return (paths);
}

void	check_err_output(t_data *data, t_stack **stack,
			char **paths, char *slash_path)
{
	t_redir	*redir;

	redir = (*stack)->node->redir;
	while (redir)
	{
		if (redir->type == REDIR_OUT && redir->fd == 2)
		{
			redir->out_fd = open(redir->file, O_WRONLY
					| O_CREAT | O_TRUNC, 0644);
			if (redir->out_fd == -1)
			{
				system_error(strerror(errno), redir->file);
				ft_splitfree(paths);
				executor_cleanup(data, stack, slash_path);
				exit(1);
			}
			dup2(redir->out_fd, redir->fd);
			close(redir->out_fd);
			break ;
		}
		redir = redir->next;
	}
}

char	*cmd_not_found(t_data *data, t_stack **stack,
			char **paths, char *slash_path)
{
	check_err_output(data, stack, paths, slash_path);
	internal_error(INT_ERR_6,
		(*stack)->node->argv[get_first_command(data, stack)], NULL);
	if ((*stack)->node->redir)
	{
		if ((*stack)->node->redir->type == REDIR_OUT)
			(*stack)->node->redir->fd = open((*stack)->node->redir->file,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if ((*stack)->node->redir->type == APPEND)
			(*stack)->node->redir->fd = open((*stack)->node->redir->file,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		close((*stack)->node->redir->fd);
	}
	ft_splitfree(paths);
	executor_cleanup(data, stack, slash_path);
	exit(127);
	return (NULL);
}
