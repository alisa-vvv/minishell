/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   built_ins.c                                         :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/18 17:54:14 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/18 17:54:16 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <unistd.h>

int ft_cd(element **tokenlist, int pos)
{
    if ((*tokenlist)->elementList.tokens[pos +1].value[0] == '/')
    {
        (*tokenlist)->elementList.tokens[pos +1].type = PARAMETER;
        return (chdir( (const char *)(*tokenlist)->elementList.tokens[pos +1].value));
    }
    return(-1);
}

int 