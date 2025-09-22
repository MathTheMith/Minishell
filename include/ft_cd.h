/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfournie <tfournie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 14:01:02 by tfournie          #+#    #+#             */
/*   Updated: 2025/09/11 14:01:03 by tfournie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CD_H
# define FT_CD_H

# include "minishell.h"

typedef struct s_cd_data
{
	char	*dir;
	char	cwd[1000];
	char	oldpwd[1000];
	char	*resolved_dir;
	int		arg_count;
}			t_cd_data;

// Fonction principale
void		cd_input(t_cmd *cmds);

// Fonctions utilitaires pour cd
char		*expand_tilde(char *dir, t_cmd *cmds);
char		*resolve_env_var(char *dir, t_cmd *cmds);

// Fonctions pour gestion environnement
void		add_to_env(t_list **env, char *name, char *value);

#endif