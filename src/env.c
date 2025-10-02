/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:15 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/02 17:41:44 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_valid_var_name(char *s)
{
	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	while (*s)
	{
		if (!ft_isalnum(*s) && *s !=  '_')
			return (0);
		s++;
	}
	return (1);
}

int	is_new_var(char *arg)
{
	if (!arg || (!ft_isalpha(*arg) && *arg != '_'))
		return (0);
	while (*arg && (ft_isalnum(*arg) || *arg == '_'))
		arg++;
	return (*arg == '=');
}

static t_env	*generate_env_pwd(t_data *data)
{
	t_env	*node;

	node = create_env_node(NULL, NULL, 1);
	validate_malloc(data, node, NULL);
	node->key = ft_strdup("PWD");
	validate_malloc_env(data, node->key, node);
	node->value = getcwd(NULL, 0);
	if (!node->value)
	{
		system_error(data, "getcwd");
		free_env_node(&node);
		error_exit(data);
	}
	return (node);
}

int	generate_minimal_env(t_data *data, char **argv)
{
	t_env	*pwd;
	t_env	*shlvl;
	t_env	*underscore;

	pwd = generate_env_pwd(data);
	add_env_node(&data->env_list, pwd);
	shlvl = create_env_node(NULL, NULL, 1);
	validate_malloc(data, shlvl, NULL);
	shlvl->key = ft_strdup("SHLVL");
	validate_malloc_env(data, shlvl->key, shlvl);
	shlvl->value = ft_strdup("1");
	validate_malloc_env(data, shlvl->value, shlvl);
	add_env_node(&data->env_list, shlvl);
	underscore = create_env_node(NULL, NULL, 1);
	validate_malloc(data, underscore, NULL);
	underscore->key = ft_strdup("_");
	validate_malloc_env(data, underscore->key, underscore);
	underscore->value = ft_strdup(argv[0]);
	validate_malloc_env(data, underscore->value, underscore);
	add_env_node(&data->env_list, underscore);
	return (0);
}

char	*get_env_value(t_env *head, char *key)
{
	while (head)
	{
		if (!ft_strcmp(head->key, key))
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

void	set_env_value(t_env *head, char *key, char *new_value)
{
	while (head)
	{
		if (!ft_strcmp(head->key, key))
		{
			free(head->value);
			head->value = new_value;
			return ;
		}
		head = head->next;
	}
}

void	unset_env(t_env **head, char *key)
{
	t_env	*trav;
	t_env	*tmp;

	if (!head || !*head)
		return ;
	while (*head && !ft_strcmp((*head)->key, key))
	{
		tmp = *head;
		*head = (*head)->next;
		free_env_node(&tmp);
	}
	trav = *head;
	while (trav && trav->next)
	{
		if (!ft_strcmp(trav->next->key, key))
		{
			tmp = trav->next;
			trav->next = trav->next->next;
			free_env_node(&tmp);
		}
		else
			trav = trav->next;
	}
}
