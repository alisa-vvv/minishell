/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   debug.h                                             :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/07/08 16:45:09 by adecheri       #+#    #+#                */
/*   Updated: 2025/07/08 16:45:10 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */


#ifndef DEBUG_H
# define DEBUG_H

# define DEBUG_ON 0
# define LEX_ON 0
# define EXP_ON 1
# define QUOTE_ON 0

# include <stdlib.h>
# include <stdio.h>

void    e_printf(const char *str, ...);
void    l_printf(const char *str, ...);
void    t_printf(const char *str, ...);
void    q_printf(const char *str, ...);

#endif