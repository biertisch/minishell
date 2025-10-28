/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:32:13 by beatde-a          #+#    #+#             */
/*   Updated: 2025/10/28 16:33:39 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

//env.c
int			generate_minimal_env(t_data *data, char **argv);
void		unset_env(t_env **head, char *key);
void		set_env_value(t_env *head, char *key, char *new_value);
char		*get_env_value(t_env *head, char *key);

//env_convert.c
void		env_list_to_array(t_data *data);
int			envp_to_list(t_data *data, char **envp, char **argv);
void		split_env_entry(t_data *data, char *entry, t_env *node);

//env_list.c
void		free_env_list(t_env **head);
void		free_env_node(t_env **node);
t_env		*get_last_env_node(t_env *head);
void		add_env_node(t_env **head, t_env *new_node);
t_env		*create_env_node(char *key, char *value, int exported);

//env_utils.c
int			is_valid_var_name(char *s);
int			is_new_var(char *arg);

#endif
