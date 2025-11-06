/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:16:22 by beatde-a          #+#    #+#             */
/*   Updated: 2025/11/06 14:45:03 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_exit(t_data *data, t_stack **stack)
{
	write(2, "fatal error: leaving minishell...\n", 34);
	free_stack(stack);
	free_all(data);
	exit(EXIT_FAILURE);
}

int	syntax_error(t_data *data, char *desc, char *token)
{
	char	err_msg[ERR_BUFFER_SIZE];

	err_msg[0] = '\0';
	if (ft_strncmp(desc, "minishell:", 10))
		ft_strlcpy(err_msg, "minishell: ", ERR_BUFFER_SIZE);
	if (desc)
		ft_strlcat(err_msg, desc, ERR_BUFFER_SIZE);
	append_postfix(err_msg, token);
	ft_strlcat(err_msg, "\n", ERR_BUFFER_SIZE);
	write(2, err_msg, ft_strlen(err_msg));
	data->exit_status = 2;
	return (INVALID);
}

int	internal_error(char *desc, char *cmd, char *arg)
{
	char	err_msg[ERR_BUFFER_SIZE];

	err_msg[0] = '\0';
	if (ft_strncmp(desc, "minishell:", 10))
		ft_strlcpy(err_msg, "minishell: ", ERR_BUFFER_SIZE);
	append_prefix(err_msg, cmd);
	if (arg && arg[0] != '\'')
		append_prefix(err_msg, arg);
	if (desc)
		ft_strlcat(err_msg, desc, ERR_BUFFER_SIZE);
	if (arg && arg[0] == '\'')
		append_postfix(err_msg, arg);
	ft_strlcat(err_msg, "\n", ERR_BUFFER_SIZE);
	write(2, err_msg, ft_strlen(err_msg));
	return (INVALID);
}

int	system_error(char *desc, char *function)
{
	char	err_msg[ERR_BUFFER_SIZE];

	err_msg[0] = '\0';
	if (ft_strncmp(desc, "minishell:", 10))
		ft_strlcpy(err_msg, "minishell: ", ERR_BUFFER_SIZE);
	append_prefix(err_msg, function);
	if (desc)
		ft_strlcat(err_msg, desc, ERR_BUFFER_SIZE);
	ft_strlcat(err_msg, "\n", ERR_BUFFER_SIZE);
	write(2, err_msg, ft_strlen(err_msg));
	return (INVALID);
}

void	append_prefix(char *msg, char *label)
{
	if (label)
	{
		ft_strlcat(msg, label, ERR_BUFFER_SIZE);
		ft_strlcat(msg, ": ", ERR_BUFFER_SIZE);
	}
}

void	append_postfix(char *msg, char *label)
{
	if (label)
	{
		ft_strlcat(msg, " ", ERR_BUFFER_SIZE);
		if (label[0] != '\'')
			ft_strlcat(msg, "\'", ERR_BUFFER_SIZE);
		ft_strlcat(msg, label, ERR_BUFFER_SIZE);
		if (label[0] != '\'')
			ft_strlcat(msg, "'", ERR_BUFFER_SIZE);
	}
}

void	validate_malloc_execute(t_data *data, t_stack **stack,
	void *ptr, void *to_free)
{
	if (!ptr)
	{
		close_all_open_redir_ends(data);
		close_all_pipe_ends(stack);
		system_error(strerror(errno), "malloc");
		if (to_free)
			free(to_free);
		error_exit(data, stack);
	}
}

void	validate_malloc(t_data *data, void *ptr, void *to_free)
{
	if (!ptr)
	{
		system_error(strerror(errno), "malloc");
		free(to_free);
		error_exit(data, NULL);
	}
}

void	validate_malloc_env(t_data *data, void *ptr, t_env *node)
{
	if (!ptr)
	{
		system_error(strerror(errno), "malloc");
		free_env_node(&node);
		error_exit(data, NULL);
	}
}
