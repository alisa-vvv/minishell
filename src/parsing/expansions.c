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
    check_token = (t_token *)(*tokenlist)->element_list.tokens[pos];
    
    char *name;
    name = ft_strdup(&check_token->value[1]);
    e_printf("NAME= %s \n", name);
    if (env_var_get_value((*minishell_data)->environment, name))
    {
        e_printf("VALUE= %s \n", env_var_get_value((*minishell_data)->environment, name));
        check_token->value = env_var_get_value((*minishell_data)->environment, name);
    }
    else 
    {
        (*tokenlist)->pf_element_delete((*tokenlist), pos);
        index_lexer(tokenlist);
        return (1);
    }
    free(name);
    return (0);
}
 