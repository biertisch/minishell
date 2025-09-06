/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:16:22 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/26 14:16:22 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	validate_malloc(t_data *data, void *ptr, void *to_free)
{
	if (!ptr)
	{
		report_error("malloc", SYSTEM_ERR);
		free(to_free);
		error_exit(data);
	}
}

void	validate_malloc_env(t_data *data, void *ptr, t_env *node)
{
	if (!ptr)
	{
		report_error("malloc", SYSTEM_ERR);
		free_env_node(&node);
		error_exit(data);
	}
}

void	validate_malloc_tree(t_data *data, void *ptr, t_tree *left,
	t_tree *right)
{
	if (!ptr)
	{
		report_error("malloc", SYSTEM_ERR);
		free_parser_tree(&left);
		free_parser_tree(&right);
		error_exit(data);
	}
}

<<<<<<< HEAD
void	error_exit(t_data *data)
{
	write(2, "fatal error: leaving minishell...\n", 33);
=======
void	validate_malloc_wildcard(t_data *data, void *ptr, t_list *node,
	char **new_argv)
{
	if (!ptr)
	{
		report_error("malloc", SYSTEM_ERR);
		ft_lstclear(&node, free);
		free_string_array(&new_argv);
		error_exit(data);
	}
}

void	error_exit(t_data *data)
{
	write(2, "fatal error: leaving minishell...\n", 34);
>>>>>>> origin/dev
	free_all(data);
	exit(EXIT_FAILURE);
}

int	report_error(char *error_msg, t_error error_code)
{
	if (!error_msg)
		error_msg = "unkown error";
	if (error_code == SYSTEM_ERR)
	{
		write(2, "system error: ", 14);
		perror(error_msg);
	}
	else
	{
		if (error_code == SYNTAX_ERR)
			write(2, "syntax error: ", 14);
		else
			write(2, "internal error: ", 16);
		write(2, error_msg, ft_strlen(error_msg));
		write(2, "\n", 1);
	}
	return (-1);
}
<<<<<<< HEAD
//error_exit error.c
void	check_for_errors(int status, t_data *data, t_stack *stack, char *command_name)
{
	if (status == -1)
	{
		perror(command_name);
		free_stack(stack);
		free_all(data);
		exit(EXIT_FAILURE);
	}
}

void	free_stack(t_stack *stack)
{
	t_stack	*next;
	t_stack	*old_next;

	old_next = stack;
	next = stack->next;
	while (next)
	{
		free(old_next);
		old_next = next;
		next = next->next;
	}
	free(old_next);
}
=======
>>>>>>> origin/dev
