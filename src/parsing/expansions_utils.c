/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   expansions_utils.c                                  :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/11/04 15:52:14 by adecheri       #+#    #+#                */
/*   Updated: 2025/11/04 15:52:16 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


char *prep_leftover(
    char *str_token, 
    int offset)
{
    char *start; 
    char *temp_left;

    start = NULL;
    temp_left = NULL;
    if (!str_token)
        return(NULL);
    start = ft_strrchr(str_token, '$');
    if (!start)
        return (NULL);
    if (ft_strlen(start) - offset)
        temp_left = malloc(sizeof(char) * (ft_strlen(start) - offset));
    else 
        temp_left = malloc(sizeof(char) * (ft_strlen(start) - offset)+1);
    if (!temp_left)
        return (NULL);
    ft_strlcpy(temp_left, start + offset, (ft_strlen(start)- offset) + 1);
    start[0] ='\0';
    return(temp_left);
}

//expand the var when token is in quotes 
char *exp_str_token(
    char *str_token, 
    char *value, 
    int offset)
{
    char *new_str;
    char *leftover;
    char *temp_right;

    temp_right = NULL;
    temp_right = prep_leftover(str_token, offset);
    leftover = ft_strjoin(value, temp_right);
    new_str = ft_strjoin(str_token, leftover);
    if (!new_str || !leftover)
        return ((ft_safe_free((unsigned char **)&leftover), ft_safe_free((unsigned char **)&value), ft_safe_free((unsigned char **)&new_str), ft_safe_free((unsigned char **)&temp_right), NULL));
    (ft_safe_free((unsigned char **)&leftover), ft_safe_free((unsigned char **)&value), ft_safe_free((unsigned char **)&str_token), ft_safe_free((unsigned char **)&temp_right));
    return (new_str);
}

int ft_isall_upper(const char *name)
{
    while(*name)
    {
        if (ft_islower(*name))
            return(1);
        name++;
    }
    return (0);
}


//get name of env var from token_name
char *refine_name_var(char *token_name, char *result)
{
    char *start;
    int i;

    i = 0;
    start = NULL;
    start = ft_strrchr(token_name, '$');
    // e_printf("\nSTART = %s\n", start);
    result = ft_strdup(start + 1);
    if (!result)
        return(NULL);
   // e_printf("\nRESULT = %s$\n", result);
    while (result[i])
    {
        if (result[i] == '"' || result[i] == ' ' || result[i] == '$' || (result[i] >= '\t' && result[i] <= '\r') || ft_islower(result[i]))
            break;
        i++;
    }
    result[i] = '\0';
    return (result);
}
