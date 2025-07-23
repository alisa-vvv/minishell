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


//expand known var and otherwise delete and re-position all tokens
int expand_var(element **tokenlist, int pos, t_minishell_data **minishell_data)
{
    t_token *check_token;
    int i;

    i = 0;
    check_token = (t_token *)(*tokenlist)->pf_element_get((*tokenlist), pos);
    
    if (env_var_get_value((*minishell_data)->environment, check_token->value))
    {
        e_printf("VALUE= %d \n", env_var_get_value((*minishell_data)->environment, check_token->value));	
        *check_token->value = *env_var_get_value((*minishell_data)->environment, check_token->value);
    }
    else 
    {
        (*tokenlist)->pf_element_delete((*tokenlist), pos);
        index_lexer(tokenlist);
    }
    

    return (0);
}
 