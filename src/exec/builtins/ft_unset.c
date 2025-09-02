/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 00:57:31 by marvin            #+#    #+#             */
/*   Updated: 2025/06/04 00:57:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction pour supprimer un noeud de votre liste circulaire
bool remove_env_var(char *name, t_list **env)
{
    t_list *tmp;
    t_list *to_delete;
    int name_len = 0;
    // int i = 0;

    (void)to_delete;
    if (!*env || !name)
        return false;

    // Calculer la longueur du nom de variable
    while (name[name_len])
        name_len++;

    tmp = *env;
    
    // Parcourir la liste circulaire
    while (tmp)
    {
        // Vérifier si c'est la variable à supprimer
        if (!ft_strncmp(tmp->str, name, name_len) && 
            (tmp->str[name_len] == '\0' || tmp->str[name_len] == '='))
        {
            to_delete = tmp;
            
            // Cas spécial : un seul élément dans la liste
            if (tmp->next == tmp && tmp->prev == tmp)
            {
                free(tmp->str);
                free(tmp);
                *env = NULL;
                return true;
            }
            
            // Réajuster les pointeurs
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
            
            // Si on supprime le premier élément, déplacer le pointeur env
            if (*env == tmp)
                *env = tmp->next;
            
            // Libérer la mémoire
            free(tmp->str);
            free(tmp);
            return true;
        }
        
        tmp = tmp->next;
        
        // Éviter la boucle infinie dans la liste circulaire
        if (tmp == *env)
            break;
    }
    
    return false; // Variable non trouvée
}

// Fonction unset mise à jour
void unset_input(t_cmd *cmds, t_list **env)
{
    int i = 1;
    
    if (ft_strcmp(cmds->args[0], "unset") != 0)
        return;
    
    // Si pas d'arguments, c'est valide mais ne fait rien
    if (!cmds->args[1])
    {
        cmds->last_exit_code = 0;
        return;
    }
    
    while (cmds->args[i])
    {
        // Vérifier si le nom de variable est valide (réutiliser votre fonction)
            // Supprimer de votre liste d'environnement personnalisée
            remove_env_var(cmds->args[i], env);
            
            // Supprimer aussi de l'environnement système
            unsetenv(cmds->args[i]);
        i++;
    }
    
    cmds->last_exit_code = 0;
}
