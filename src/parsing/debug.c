/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   debug.c                                             :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/07/08 16:45:21 by adecheri       #+#    #+#                */
/*   Updated: 2025/07/08 16:45:23 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */


#include "debug.h"

void e_printf(const char *str, ...)
{
    if (!EXP_ON)
        return;
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
}
void l_printf(const char *str, ...)
{
    if (!LEX_ON)
        return;
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
}


void t_printf(const char *str, ...)
{
    if (!DEBUG_ON)
        return;
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
}

void q_printf(const char *str, ...)
{
    if (!QUOTE_ON)
        return;
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
}

