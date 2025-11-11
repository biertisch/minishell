/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirect2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedde-so <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 12:24:01 by pedde-so          #+#    #+#             */
/*   Updated: 2025/11/11 12:24:02 by pedde-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redir_out_ok(t_data *data, t_redir *redir, t_list *new, int *fd)
{
	fd = malloc(sizeof(int));
	validate_malloc(data, fd, NULL);
	*fd = redir->out_fd;
	new = ft_lstnew(fd);
	validate_malloc(data, new, fd);
	if (!data->open_redirs)
		data->open_redirs = new;
	else
		ft_lstadd_back(&data->open_redirs, new);
	if (redir->out_fd == -1)
	{
		handle_open_errors(redir);
		return (2);
	}
	return (1);
}

int	open_redir_out(t_data *data, t_redir *redir, t_list *new, int *fd)
{
	if (expand_single_redir(data, redir))
	{
		redir->out_fd = -1;
		return (2);
	}
	if (redir->type == REDIR_OUT)
		redir->out_fd = open(redir->file, O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
	else
		redir->out_fd = open(redir->file, O_WRONLY
				| O_CREAT | O_APPEND, 0644);
	if (redir->fd != -1)
	{
		duplicate_std();
		dup2(redir->out_fd, redir->fd);
		close(redir->out_fd);
	}
	else if (open_redir_out_ok(data, redir, new, fd) == 2)
		return (2);
	return (1);
}

int	open_redir(t_data *data, t_redir *redir)
{
	int		*fd;
	t_list	*new;

	fd = NULL;
	new = NULL;
	if (!redir)
		return (0);
	while (redir)
	{
		if (redir->type == REDIR_IN
			&& (open_redir_in(data, redir, new, fd) == 2))
			return (2);
		if ((redir->type == REDIR_OUT || redir->type == APPEND)
			&& (open_redir_out(data, redir, new, fd) == 2))
			return (2);
		redir = redir->next;
	}
	return (1);
}

int	close_redir_in(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			if (redir->in_fd == -1)
				break ;
			close(redir->in_fd);
		}
		redir = redir->next;
	}
	return (1);
}

void	handle_open_errors(t_redir *redir)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, redir->file, ft_strlen(redir->file));
	if (errno == ENOENT)
		write(STDERR_FILENO, ": No such file or directory\n", 28);
	else if (errno == EACCES)
		write(STDERR_FILENO, ": Permission denied\n", 20);
	else if (errno == EISDIR)
		write(STDERR_FILENO, ": Is a directory\n", 17);
}
