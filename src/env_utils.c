/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:24:26 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/30 15:17:28 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_name(char *s)
{
	if (!s || (!ft_isalpha(*s) && *s != '_'))
		return (0);
	while (*s)
	{
		if (!ft_isalnum(*s) && *s != '_')
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

void	increment_shlvl(t_data *data, t_env *head)
{
	char	*current_level;
	char	*new_level;
	t_env	*node;

	node = find_env_node(head, "SHLVL");
	current_level = get_env_value(head, "SHLVL");
	if (!node || !current_level)
		return ;
	new_level = ft_itoa(ft_atoi(current_level) + 1);
	validate_malloc(data, new_level, NULL);
	free(node->value);
	node->value = new_level;
}

t_env	*find_env_node(t_env *head, char *key)
{
	if (!head)
		return (NULL);
	while (head)
	{
		if (!ft_strcmp(head->key, key))
			return (head);
		head = head->next;	
	}
	return (NULL);
}
