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
# define EXP_ON 0
# define QUOTE_ON 0
# define PARSE_ON 1

# include <stdlib.h>
# include <stdio.h>
# include "vec_lib.h"
# include "parser.h"
# include "minishell.h"


void    e_printf(const char *str, ...);
void    l_printf(const char *str, ...);
void    t_printf(const char *str, ...);
void    p_printf(const char *str, ...);
void    q_printf(const char *str, ...);
void    test_tokens(element tokenlist);
void    test_execdata(t_exec_data execdata, size_t n);
void    test_minishelldata(t_minishell_data minishell_data);

#endif
