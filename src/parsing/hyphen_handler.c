/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   hyphen_handler.c                                    :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/08/11 13:51:40 by adecheri       #+#    #+#                */
/*   Updated: 2025/08/11 13:51:42 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

//check to see if string is all alph or digits
int all_num_alph(
    const char *str)
{
    size_t  i;

    i = 0;
    if (!str)
        return (UNKNOWN);
    if (ft_isalpha(str[i]))
    {
        while(str[i] && ft_isalpha(str[i]))
            i++;
        if (str[i] == '\0')
            return (STRING);
        else 
            return (UNKNOWN);
    } 
    else if (ft_isdigit(str[i]))
    { 
        while (str[i] && ft_isdigit(str[i]))
            i++;
        if (str[i] == '\0')
            return (NUMBER);
        else 
            return(UNKNOWN);
    }
    return(UNKNOWN);
}

int check_hyphens(
    const char *str_token)
{
    if (str_token + 1 == (void*)0)
        return (OPERATOR);
    else if (all_num_alph(str_token + 1) == STRING)
        return(HYPHEN);
    else 
        return (all_num_alph(str_token + 1));
}
