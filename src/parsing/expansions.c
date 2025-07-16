/* ************************************************************************** */
/*                                                                            */
/*                                                         ::::::::           */
/*   expansions.c                                        :+:    :+:           */
/*                                                      +:+                   */
/*   By: adecheri <marvin@42.fr>                       +#+                    */
/*                                                    +#+                     */
/*   Created: 2025/06/17 14:24:03 by adecheri       #+#    #+#                */
/*   Updated: 2025/06/17 14:24:05 by adecheri       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"


// $ expansion, environment
// tilde expansion?
// $0 name of the shell script or command
// $1-9 indicated postiional arg 
// $# number of shell args
// $@ value of each shell arg 
// $* value of all shell arg

//var expansion: $sign, $@sign 
//path expansion
//--> make expansion first, remove quotes later
// expand $EMPTY to nothing
// look for return value in minishell struct when accessing $?
// 
 
char *check_var(element **tokenlist, int pos)
{
    t_token *check_token;

    check_token = (t_token *)(*tokenlist)->element_list.tokens[pos];
    if (check_token->type == PARAMETER)
    {
        
    }
}
