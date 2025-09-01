/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 10:38:18 by beatde-a          #+#    #+#             */
/*   Updated: 2025/08/20 10:38:18 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	remove_outer_quotes(t_data *data, char **arg)
{
	char	*set;
	char	*tmp;

	if (**arg == '\'')
		set = "'";
	else if (**arg == '"')
		set = "\"";
	else
		return ;
	tmp = ft_strtrim(*arg, set);
	validate_malloc(data, tmp, NULL);
	free(*arg);
	*arg = tmp;
}

/* static int	expand_wildcard(t_data *data, char **arg)
{

} */

static void	expand_exit_status(t_data *data, char **arg, int i)
{
	char	*value;

	value = ft_itoa(data->last_exit_status);
	validate_malloc(data, value, NULL);
	*arg = replace_key_value(*arg, i, "?", value);
	validate_malloc(data, *arg, value);
	free(value);
}

static void	expand_variable(t_data *data, char **arg, int i)
{
	char	*key;
	char	*value;

	key = get_env_key((*arg) + i + 1);
	validate_malloc(data, key, NULL);
	value = get_env_value(data->env_list, key);
	if (!value)
		value = "";
	*arg = replace_key_value(*arg, i, key, value);
	validate_malloc(data, *arg, key);
	free(key);
}

static void	expand_dollar(t_data *data, char **arg)
{
	int	i;

	i = 0;
	while ((*arg)[i])
	{
		if ((*arg)[i] == '$')
		{
			if ((*arg)[i + 1] == '?')
				expand_exit_status(data, arg, i);
			else
				expand_variable(data, arg, i);
			i = -1;
		}
		i++;
	}
}

int	expand(t_data *data, t_tree *node)
{
	int	i;

	if (!node)
		return (-1);
	if (node->type == NODE_CMD && node->argv)
	{
		i = 0;
		while (node->argv[i])
		{
			if (node->argv[i][0] != '\'')
			{
				expand_dollar(data, &node->argv[i]);
				//if (expand_wildcard(data, &node->argv[i]))
				//	return (-1);
			}
			remove_outer_quotes(data, &node->argv[i]);
			i++;
		}
	}
	expand(data, node->left);
	expand(data, node->right);
	return (0);
}

// (if not single quotes) expand wildcards * in current directory
// if wildcard is undefined, report error and return 